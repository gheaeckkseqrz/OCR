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
  unsigned int getNeuronsCount(unsigned int layerId  = -1) const;
  unsigned int getSynapsesCount(unsigned int layerId = -1) const;
  std::vector<unsigned int> getLayerDescription() const;
  std::string getDescription() const;
  std::string getDescription(unsigned int layer) const;

 public:
  static const unsigned int PICTURE_RESOLUTION;

 private:
  typedef std::vector<Neuron *> NeuronLayer;
  std::vector<NeuronLayer> _neurons;
};

#endif
