#include <math.h>       /* fabs */
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <numeric>
#include "Gene.h"

Gene::Gene(std::string const &path)
{
  if (!path.empty())
    loadFromFile(path);
}

void Gene::loadFromFile(std::string const &path)
{
  std::ifstream t(path);
  if (t.is_open())
    {
      float w(0);
      while (t >> w)
	_weights.push_back(w);
      std::cout << "Loaded " << _weights.size() << " from " << path << std::endl;
    }
  else
    std::cerr << "Failled to open " << path << std::endl;
}

void Gene::clear()
{
  _weights.clear();
  _fullResults.clear();
  _bitResults.clear();
}

void Gene::addWeight(float w)
{
  _weights.push_back(w);
}

float Gene::getWeight(unsigned int index) const
{
  if (index >= _weights.size())
    {
      std::cerr << "Invalid index for gene access (" << index << "/" << _weights.size() << ")" << std::endl;
    }
  return _weights[index];
}

void Gene::mute(unsigned int probability)
{
  for (unsigned int i(0) ; i < _weights.size() ; ++i)
    if (rand() % 100 < probability)
      _weights[i] = (rand() % 2) ? ((float)((rand() % 2001) - 1000) / 1000) : 0;
}

void Gene::mute(Gene const &model, unsigned int probability)
{
  for (unsigned int i(0) ; i < _weights.size() ; ++i)
    if (rand() % 100 < probability)
      _weights[i] = model._weights[i];
}

void Gene::setFullResults(std::vector<unsigned int> const &results)
{
  _fullResults = results;
}

void Gene::setBitResults(std::vector<unsigned int> const &results)
{
  _bitResults = results;
}

void Gene::setOutput(std::vector<unsigned int> const &output)
{
  _output = output;
}

std::string Gene::printResults(std::vector<bool> const &dataset, unsigned int fontsCount, bool color) const
{
  const char *GREEN = "\033[0;32m";
  const char *BLUE = "\033[0;34m";
  const char *RED = "\033[0;31m";
  const char *RESET = "\033[0;00m";

  std::stringstream ss;
  for (char c('A') ; c <= 'Z' ; ++c)
    {
      if (_fullResults.size() > c - 'A' && _bitResults.size() > c - 'A' && dataset[c - 'A'])
	{
	  if (_fullResults[c - 'A'] == fontsCount && color)
	    ss << GREEN;
	  else if (_fullResults[c - 'A'] > 0 && color)
	    ss << BLUE;
	  ss << "[" << c << "|" << _fullResults[c - 'A'] << "|" << _bitResults[c - 'A'] << "]";
	  if (color)
	    ss << RESET;
	}
    }
  ss << " => " << getScore() << " (" << getWeightsSum() << ")";
  std::string s = ss.str();
  return s;
}

float Gene::getScore() const
{
   float score(0);

//     unsigned int unique(0);
//     for (auto f : _fullResults)
//       {
//         score += 10 * f;
//         unique += f ? 1 : 0;
//       }
  for (auto b : _bitResults)
    score += b;
  return score;// * (unique + 1);
}

float Gene::getWeightsSum() const
{
  float sum(0);
  for (auto w : _weights)
    sum += fabs(w);
  return sum;
}

void Gene::saveToFile(std::string const &path)
{
  std::stringstream ss;

  for (auto w : _weights)
    ss << w << " ";

  std::ofstream out(path);
  out << ss.str();
  out.close();
}

bool Gene::perfect(std::vector<bool> const &dataset, unsigned int fontsCount) const
{
  if (dataset.size() != _fullResults.size() || dataset.empty())
    return false;
  for (unsigned int i(0) ; i < dataset.size() ; ++i)
    {
      if (dataset[i] && _fullResults[i] != fontsCount)
	return false;
    }
  return true;
}
