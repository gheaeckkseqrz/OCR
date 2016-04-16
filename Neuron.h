#ifndef __NEURON_H__
#define __NEURON_H__

#include <iostream>
#include <vector>
class Gene;

class Neuron
{
 public:
  typedef std::pair<Neuron *, float> Synapse_t;

 public:
  Neuron();
  virtual ~Neuron();

  unsigned int getId() const;
  void reset();
  virtual float getOutput();
  void connectSynapse(Synapse_t const &s);

  void adjustWeights(float diff);

  float sigmoid(float v);
  float sigmoidPrime(float v);

  // private:
  unsigned int _id;
  bool _computed;
  float _value;
  float _sum;
  std::vector<Synapse_t> _inputs;
  std::vector<Synapse_t> _oldInputs;
};

std::ostream& operator <<(std::ostream& osObject, const Neuron& n);

#endif
