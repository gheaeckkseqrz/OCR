#include <sstream>
#include <iostream>
#include <thread>
#include "Network.h"
#include "InputNeuron.h"
#include "Gene.h"

Network::Network(Manager const &manager)
{
  const unsigned int PICTURE_RESOLUTION = 16;
  const unsigned int INPUT_NEURONS = PICTURE_RESOLUTION * PICTURE_RESOLUTION;
  const unsigned int OUTPUT_NEURONS = 8;
  const unsigned int HIDDEN_LAYERS = 2;

  _neurons.push_back(NeuronLayer()); // Input Layer
  for (unsigned int i(0) ; i < INPUT_NEURONS ; ++i)
    _neurons.back().push_back(new InputNeuron(manager, i / PICTURE_RESOLUTION, i % PICTURE_RESOLUTION));

  for (unsigned int layer(1) ; layer <= HIDDEN_LAYERS ; ++layer)
    {
      _neurons.push_back(NeuronLayer());
      for (unsigned int i(0) ; i < INPUT_NEURONS ; ++i)
	addNeuron(layer);
    }

  _neurons.push_back(NeuronLayer());
  for (unsigned int i(0) ; i < OUTPUT_NEURONS ; ++i)
	addNeuron(HIDDEN_LAYERS + 1);
}

Neuron *Network::addNeuron(unsigned int layer)
{
  if (layer > _neurons.size())
    {
      std::cerr << "Trying to add neuron to non-existing layer " << layer << std::endl;
      return nullptr;
    }
  if (layer == 0)
  {
    std::cerr << "Can't add input/output neuron here" << std::endl;
    return nullptr;
  }
  
  Neuron *neuron = new Neuron();
  _neurons[layer].push_back(neuron);
  for (auto n : _neurons[layer - 1])
    neuron->connectSynapse(Neuron::Synapse_t(n, 0));
  if (_neurons.size() > layer + 1)
    {
      for (auto n : _neurons[layer + 1])
	n->connectSynapse(Neuron::Synapse_t(neuron, 0));
    }

  return neuron;
}

Neuron *Network::getNeuron(unsigned int id) const
{
  for (auto l : _neurons)
    for (auto n : l)
      if (n->getId() == id)
	return n;
  return nullptr;
}

void Network::reset()
{
  for (auto l : _neurons)
    for (auto n : l)
      n->reset();
}

unsigned char Network::getOutput()
{
  unsigned char result(0);
  reset();

  if (_neurons.back().size() != 8 )
    {
      std::cerr << "Last layer should be 8 neurons, not " << _neurons.back().size() << std::endl;
      return 0;
    }

  std::thread threads[8];
  for (int i(0) ; i < 8 ; ++i)
    threads[i] = std::thread(&Neuron::getOutput, _neurons.back()[i]);
  for (int i(0) ; i < 8 ; ++i)
    threads[i].join();

  for (auto n : _neurons.back())
    {
      result = result << 1;
      result |= (n->getOutput() > 0.5) ? 1 : 0;
    }

  return result;
}

void Network::save(Gene &gene)
{
  gene.clear();
  for (auto l : _neurons)
    for (auto n : l)
      n->saveWeights(gene);
}

void Network::load(Gene &gene)
{
  if (getSynapsesCount() != gene._weights.size())
    {
      std::cerr << "Can't load gene with size " << gene._weights.size() << " (Network size : " << getSynapsesCount() << ")" << std::endl;
      return;
    }

  unsigned int index(0);
  for (auto l : _neurons)
    for (auto n : l)
      index = n->loadWeights(gene, index);
}

unsigned int Network::getNeuronsCount() const
{
  unsigned int c(0);
  for (auto l : _neurons)
    c += l.size();
  return c;
}

unsigned int Network::getSynapsesCount() const
{
  unsigned int previousSize = 0;
  unsigned int c(0);
  for (auto l : _neurons)
    {
      c += previousSize * l.size();
      previousSize = l.size();
    }
  return c;
}

std::string Network::getDescription() const
{
  std::string s;
  for (unsigned int l(0) ; l < _neurons.size() ; ++l)
    s += "Layer " + std::to_string(l) + " : " + std::to_string(_neurons[l].size()) + " neurons\n"; 
  return s;
}

std::string Network::getDescription(unsigned int layer) const
{
  std::stringstream ss;
  for (auto n : _neurons[layer])
    ss << *n;
  return ss.str();
}


