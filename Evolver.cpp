#include <iostream>
#include "Evolver.h"

Evolver::Evolver()
{
}

Gene &Evolver::getBestGene()
{
  return _best;
}

Gene &Evolver::getGene(unsigned int index)
{
  return _genes[index];
}

void Evolver::evolve(unsigned int iteration)
{
  const char *GREEN = "\033[0;32m";
  const char *RESET = "\033[0;00m";
  
  static unsigned int lastIncrease(0);
  float increase(0);
  std::sort(std::begin(_genes), std::end(_genes), [](Gene const &a, Gene const &b) {
      return a.getScore() > b.getScore();
    });

  lastIncrease++;
  if (_genes[0].getScore() > _best.getScore() || (_genes[0].getScore() == _best.getScore() && _genes[0].getWeightsSum() < _best.getWeightsSum()))
    {      
      increase = _genes[0].getScore() - _best.getScore();
      _best = _genes[0];
      lastIncrease = 0;
      _best.saveToFile(std::string("save/iteration") + std::to_string(iteration) + ".txt");
      std::cout << "Best score is " << _best.getScore() << " (" << GREEN << "+" << increase << RESET << ")." << std::endl;
    }
  else
    std::cout << "Best score is " << _best.getScore() << " (" << _best.getWeightsSum() << ") (last increase " << lastIncrease << " turns ago)." << std::endl;

  _genes[1] = _best;
  _genes[1].mute(_genes[0]); // Do a crossover between best two

  _genes[0] = _best;
  _genes[0].mute(5); // Mute a bit the best one

  _genes[2].mute(10); // Keep muting second best one

  _genes[3] = _best; // Mute a third of best one
  _genes[3].mute(35);

  _genes[4].mute(100); // Create fresh
}

void Evolver::forgetBestScore()
{
  std::vector<unsigned int> results(26);
  _best.setBitResults(results);
  _best.setFullResults(results);
}
