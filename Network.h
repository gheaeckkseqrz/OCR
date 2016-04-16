#ifndef __NETWORK_H__
#define __NETWORK_H__

#include <map>
#include <vector>
#include "Neuron.h"
class Manager;

class Network
{
 public:
  Network(Manager const &manager);
  
  Neuron *addNeuron(unsigned int layer);
  Neuron *addOutput(std::string const &tag);
  Neuron *getNeuron(unsigned int id) const;

  void reset();
  std::map<std::string, float> getOutput();
  void adjustWeights(std::map<std::string, float> const &diffs);

  std::vector<float> computeLayer2Gradient(std::map<std::string, float> const &diffs);
  std::vector<float> computeLayer1Gradient(std::map<std::string, float> const &diffs);

  unsigned int getNeuronsCount(unsigned int layerId  = -1) const;
  unsigned int getSynapsesCount(unsigned int layerId = -1) const;
  std::vector<unsigned int> getLayerDescription() const;
  std::string getDescription() const;
  std::string getDescription(unsigned int layer) const;

 public:
  static const unsigned int PICTURE_RESOLUTION;

  // private:
  typedef std::vector<Neuron *> NeuronLayer;
  std::vector<NeuronLayer> _neurons;
  std::map<std::string, Neuron*> _outputs;
};

#endif
