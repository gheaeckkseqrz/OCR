#ifndef __PROMPT_H__
#define __PROMPT_H__

#include "Manager.h"

class Prompt
{
 public:
  Prompt(Manager &m);

  void run();
  void train();
  void add(unsigned int layer = -1, unsigned int count = -1);
  void dataset();

 private:
  Manager &_manager;
};

#endif
