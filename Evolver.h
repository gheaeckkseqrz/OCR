#ifndef __EVOLVER_H__
#define __EVOLVER_H__

#include "Gene.h"

class Evolver
{
 public:
  Evolver();

  Gene &getGene(unsigned int index);
  void evolve(unsigned int iteration = 0);

 private:
  Gene _genes[5];
  Gene _best;
};

#endif
