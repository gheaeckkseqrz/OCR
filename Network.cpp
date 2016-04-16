#include <cmath> // fabs
#include <cassert>
#include <sstream>
#include <iostream>
#include <thread>
#include "Network.h"
#include "InputNeuron.h"

const unsigned int Network::PICTURE_RESOLUTION = 32;

Network::Network(Manager const &manager)
{
   const unsigned int INPUT_NEURONS = PICTURE_RESOLUTION * PICTURE_RESOLUTION;
//   const unsigned int HIDDEN_LAYERS = 0;

  _neurons.push_back(NeuronLayer()); // Input Layer
   for (unsigned int i(0) ; i < INPUT_NEURONS ; ++i)
     _neurons.back().push_back(new InputNeuron(manager, i / PICTURE_RESOLUTION, i % PICTURE_RESOLUTION));

//   for (unsigned int layer(1) ; layer <= HIDDEN_LAYERS ; ++layer)
//     {
//       _neurons.push_back(NeuronLayer());
//       for (unsigned int i(0) ; i < INPUT_NEURONS * 2 ; ++i)
// 	addNeuron(layer);
//     }

//   _neurons.push_back(NeuronLayer());
//   for (char i('A') ; i <= 'Z' ; ++i)
//     addOutput(std::string("") + i);

//   _neurons.push_back(NeuronLayer()); // Input Layer
//   for (unsigned int i(0) ; i < 2 ; ++i)
//     _neurons.back().push_back(new InputNeuron());
  _neurons.push_back(NeuronLayer());
  for (unsigned int i(0) ; i < 250 ; ++i)
    addNeuron(1);
  _neurons.push_back(NeuronLayer());
  addOutput(std::string("Output"));
  
  _neurons[1][0]->_inputs[0].second = .8;
  _neurons[1][0]->_inputs[1].second = .2;
  _neurons[1][1]->_inputs[0].second = .4;
  _neurons[1][1]->_inputs[1].second = .9;
  _neurons[1][2]->_inputs[0].second = .3;
  _neurons[1][2]->_inputs[1].second = .5;

  _neurons[2][0]->_inputs[0].second = .3;
  _neurons[2][0]->_inputs[1].second = .5;
  _neurons[2][0]->_inputs[2].second = .9;
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
    neuron->connectSynapse(Neuron::Synapse_t(n, (float)(((long long int)n + (long long int)neuron) % 1000) / 1000 * (std::rand() % 2 ? -1 : 1)));
  if (_neurons.size() > layer + 1)
    {
      for (auto n : _neurons[layer + 1])
	n->connectSynapse(Neuron::Synapse_t(neuron, (float)(((long long int)n + (long long int)neuron) % 1000) / 1000 * (std::rand() % 2 ? -1 : 1)));
    }

  return neuron;
}

Neuron *Network::addOutput(std::string const &tag)
{
  Neuron *n = addNeuron(_neurons.size() - 1);
  _outputs[tag] = n;
  return n;
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

std::map<std::string, float> Network::getOutput()
{
  std::map<std::string, float> results;
  reset();

  for (auto o : _outputs)
    {
      results[o.first] = o.second->getOutput();
      std::cout << "[" << o.first << "] -> " << (results[o.first] > .75 ? "\033[1;32m" : (results[o.first] < .25) ? "\033[0;31m" : "") << results[o.first] << "\033[m <=> sig(" << o.second->_sum << ")" << std::endl;
    }
  return results;
}

void Network::adjustWeights(std::map<std::string, float> const &diffs)
{
  std::vector<float> layer2gradient = computeLayer2Gradient(diffs);
  std::vector<float> layer1gradient = computeLayer1Gradient(diffs);

  unsigned int i(0);
  unsigned int j(0);
  for (auto n : diffs)
    {
      Neuron *neu = _outputs[n.first];
      for (auto &w2 : neu->_inputs)
	{
	  w2.second += (layer2gradient[i] > 0 ? 0.01 : -0.01) * w2.first->getOutput();
	  for (auto &w1 : w2.first->_inputs)
	    {
	      w1.second += w1.first->getOutput() * (layer1gradient[j] > 0 ? 0.01 : -0.01);
	      j++;
	    }
	  i++;
	}
    }
}

std::vector<float> Network::computeLayer2Gradient(std::map<std::string, float> const &diffs)
{
  std::vector<float> gradient;
  int i(0);

  for (auto o : _outputs)
    {
      for (auto w : o.second->_inputs)
	{
	  float derivative = o.second->sigmoidPrime(o.second->_sum) * diffs.at(o.first);
	  //	  std::cout << "Derivative[" << o.second->_id << "/" << w.first->_id << "] = " << o.second->sigmoidPrime(o.second->_sum) << " * " << diffs.at(o.first)[0] << " => " << derivative << std::endl;
	  i++;
	  gradient.push_back(derivative);
	}
    }
  return gradient;
}

std::vector<float> Network::computeLayer1Gradient(std::map<std::string, float> const &diffs)
{
  std::vector<float> gradient;

  for (auto o : _outputs) // Looping through last layer neurons
    {
      for (auto w2 : o.second->_inputs) // Looping through Last Layer Neurons'input (aka middle layer neurons)
	{
	  for (auto w : w2.first->_inputs)
	    {
	      float derivative = diffs.at(o.first) * -1 * _outputs["Output"]->sigmoidPrime(_outputs["Output"]->_sum) * w2.first->sigmoidPrime(w2.first->_sum) * w.first->getOutput() * w.second;
	      //	      std::cout << "Derivative[" << w2.first->_id << "/" << w.first->_id << "] = " << diffs.at(o.first)[0] << " * -1 * " << _outputs["Output"]->sigmoidPrime(_outputs["Output"]->_sum) << " * " <<  w2.first->sigmoidPrime(w2.first->_sum) << " * " <<  w.first->getOutput() << " * " <<  w.second << " => " << derivative << std::endl;
	      gradient.push_back(derivative);
	    }
	}
    }
  std::cout << std::endl;
  return gradient;
}

unsigned int Network::getNeuronsCount(unsigned int layerId) const
{
  if (layerId >= _neurons.size() && layerId != -1)
    return 0;
  if (layerId != -1)
    return _neurons[layerId].size();
  unsigned int c(0);
  for (auto l : _neurons)
    c += l.size();
  return c;
}

unsigned int Network::getSynapsesCount(unsigned int layerId) const
{
  if (layerId >= _neurons.size() && layerId != -1)
    return 0;
  if (layerId != -1)
    return _neurons[layerId].size() * (layerId - 1 < layerId ? _neurons[layerId - 1].size() : 0); 
  unsigned int previousSize = 0;
  unsigned int c(0);
  for (auto l : _neurons)
    {
      c += previousSize * l.size();
      previousSize = l.size();
    }
  return c;
}

std::vector<unsigned int> Network::getLayerDescription() const
{
  std::vector<unsigned int> description;
  for (auto l : _neurons)
    description.push_back(l.size());
  return description;
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
    ss << *n << std::endl;
  return ss.str();
}


