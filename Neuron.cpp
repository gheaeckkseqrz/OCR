#include <sstream>
#include <iostream> // TODO : remove
#include <cmath>
#include "Neuron.h"

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

unsigned int Neuron::getId() const
{
  return _id;
}

void Neuron::reset()
{
  _computed = false;
  _value = 0;
}

float Neuron::getOutput()
{
  std::stringstream ss;

  if (!_computed)
    {
      _sum = 0;
      ss << "Neuron " << _id << " : ";
      for (auto s : _inputs)
	{
	  if (s.second)
	    {
	      ss << s.first->getOutput() << " * " << s.second << " + ";
	      _sum += s.first->getOutput() * s.second;
	    }
	}
      ss << " => sig(" << _sum << ")";
      _value = sigmoid(_sum);
      ss << " = " << _value << std::endl;
      _computed = true;
    }
  if (_inputs.size() < 20)
    std::cout << ss.str();
  return _value;
}

void Neuron::connectSynapse(Synapse_t const &s)
{
  _inputs.push_back(s);
}

void Neuron::adjustWeights(float diff)
{
  //  std::cout << "Adjusting weights with diff = " << diff << std::endl;
  //std::cout << "Sum : " << _sum << std::endl;

  _oldInputs = _inputs;

  float sigmoidPrimeValue = sigmoidPrime(_sum);
  float sumChange = sigmoidPrimeValue * diff;

  for (auto &s : _inputs)
    s.second += sumChange /  s.second;
}

float Neuron::sigmoid(float v)
{
  return 1.0 / (1.0 + exp(-v));
}

float Neuron::sigmoidPrime(float v)
{
  return sigmoid(v) * (1 - sigmoid(v));
}

std::ostream& operator <<(std::ostream& osObject, const Neuron& n)
{
  osObject << "[" << n.getId() << "] - ";
  for (auto i : n._inputs)
    osObject << "[" << i.second << "]";
  return osObject;
}
