#ifndef __NETWORK_H__
#define __NETWORK_H__

#include <vector>
#include "Neuron.h"
class Manager;
class Gene;

class Network
{
 public:
  Network(Manager const &manager);
  
  Neuron *addNeuron(unsigned int layer);
  Neuron *getNeuron(unsigned int id) const;

  void reset();
  unsigned char getOutput();

  void save(Gene &gene);
  void load(Gene &gene);
  unsigned int getNeuronsCount() const;
  unsigned int getSynapsesCount() const;
  std::string getDescription() const;
  std::string getDescription(unsigned int layer) const;

 private:
  typedef std::vector<Neuron *> NeuronLayer;
  std::vector<NeuronLayer> _neurons;
};

#endif
