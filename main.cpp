#include <cstdlib> // srand()
#include <iostream>
#include "Manager.h"
#include "Network.h"
#include "Evolver.h"

int main(int ac, char **av)
{
  srand(time(NULL));
  Manager m;
  Network n(m);

  Evolver e;
  for (unsigned int i(0) ; i < 5 ; ++i)
    {
      e.getGene(i).loadFromFile("weights.txt");
      //      e.getGene(i).mute(100);
    }

  unsigned int iteration(0);
  while (true)
    {
      std::cout << "Iteration " << ++iteration << std::endl;
      for (unsigned int i(0) ; i < 5 ; ++i)
	m.train(n, e.getGene(i));
      e.evolve(iteration);
    }

  return 0;
}
