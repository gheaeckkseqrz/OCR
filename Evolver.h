#ifndef __EVOLVER_H__
#define __EVOLVER_H__

#include "Gene.h"

class Evolver
{
 public:
  Evolver();

  Gene &getBestGene();
  Gene &getGene(unsigned int index);
  void evolve(unsigned int iteration = 0);

  void forgetBestScore();

 private:
  Gene _genes[5];
  Gene _best;
};

#endif
