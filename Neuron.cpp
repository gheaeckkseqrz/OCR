#include <iostream> // TODO : remove
#include <cmath>
#include "Neuron.h"
#include "Gene.h"

Neuron::Neuron()
  :_id(0), _computed(false), _value(0)
{
  static unsigned int id(0);
  _id = id;
  id++;
}

Neuron::~Neuron()
{
  
}

void Neuron::reset()
{
  _computed = false;
  _value = 0;
}

float Neuron::getOutput()
{
  if (!_computed)
    {
      float sum(0);
      for (auto s : _inputs)
	{
	  if (s.second)
	    sum += s.first->getOutput() * s.second;
	}
      _value = 1.0 / (1.0 + exp(-sum));
      _computed = true;
    }
  return _value;
}

void Neuron::connectSynapse(Synapse_t const &s)
{
  _inputs.push_back(s);
}

void Neuron::saveWeights(Gene &gene) const
{
  for (auto s : _inputs)
    gene.addWeight(s.second);
}

unsigned int Neuron::loadWeights(Gene const &gene, unsigned int index)
{
  unsigned int i(0);
  for (auto &s : _inputs)
    {
      s.second = gene.getWeight(index + i);
      i++;
    }
  return index + i;
}
