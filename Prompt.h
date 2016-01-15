#ifndef __PROMPT_H__
#define __PROMPT_H__

#include "Manager.h"

class Prompt
{
 public:
  Prompt(Manager &m);

  void run();
  void train();

 private:
  Manager &_manager;
};

#endif
