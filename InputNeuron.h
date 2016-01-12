#ifndef __INPUT_NEURON_H__
#define __INPUT_NEURON_H__

#include "Neuron.h"
class Manager;

class InputNeuron : public Neuron
{
 public:
  InputNeuron(Manager const &manager, unsigned int x, unsigned int y);

  virtual float getOutput();

 private:
  Manager const &_manager;
  unsigned int _x;
  unsigned int _y;
};

#endif
