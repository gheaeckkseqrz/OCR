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

  void saveWeights(Gene &gene) const;
  unsigned int loadWeights(Gene const &gene, unsigned int index);

 private:
  unsigned int _id;
  bool _computed;
  float _value;
  std::vector<Synapse_t> _inputs;
};

std::ostream& operator <<(std::ostream& osObject, const Neuron& n);

#endif
