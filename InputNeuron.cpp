#include "InputNeuron.h"
#include "Manager.h"

InputNeuron::InputNeuron(Manager const &manager, unsigned int x, unsigned int y)
  :_manager(manager), _x(x), _y(y)
{
}

float InputNeuron::getOutput()
{
  return _manager.getPixelValue(_x, _y);
}
