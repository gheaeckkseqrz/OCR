#include <iomanip>
#include "manager.h"
#include "Network.h"

Manager::Manager()
  :_network(*this), _train(false), _dataset(26), _fontsCount(0)
{
  //  _evolver.getGene(0).loadFromFile("weights.txt");
  _network.save(_evolver.getBestGene());
  for (unsigned int i(0) ; i < 5 ; ++i)
    {
      _network.save(_evolver.getGene(i));
    }
}

void Manager::loadImage(char c, unsigned int fontId)
{
  std::string trainingDataPath = "./trainingData/";
  trainingDataPath += c;
  trainingDataPath += '/';
  trainingDataPath += std::to_string(fontId);
  trainingDataPath += ".png";

  _image = cv::imread(trainingDataPath, cv::IMREAD_GRAYSCALE);
  if(!_image.data)
    {
      std::cerr <<  "Could not open image : " << trainingDataPath << std::endl;
      return;
    }
  cv::resize(_image, _image, cv::Size(Network::PICTURE_RESOLUTION, Network::PICTURE_RESOLUTION));
}

float Manager::getPixelValue(unsigned int x, unsigned int y) const
{
  if (x < _image.rows && y < _image.cols)
    {
      return 1.0 - ((float)_image.at<unsigned char>(x, y) / 255);
    }
  std::cerr << "Invalid pixel coordinate [" << x << "/" << y << "]" << std::endl;
  std::cerr << "Image size is [" << _image.rows << "/" << _image.cols << "]" << std::endl;
  return 0;
}

void Manager::displayInput()
{
  for (unsigned int x(0) ; x < _image.cols ; ++x)
    {
    for (unsigned int y(0) ; y < _image.rows ; ++y)
      std::cout << (getPixelValue(x, y) > .1 ? "[\033[0;34m" : "[") << std::fixed << std::setprecision(1) << getPixelValue(x, y) << "\033[0;00m]";
    std::cout << std::endl;
    }
}

unsigned int Manager::getMatchingBits(char c1, char c2) const
{
  unsigned int c(0);
  for (unsigned int i(0) ; i < 8 ; ++i)
    {
      char mask(1 << i);
      c += ((c1 & mask) == (c2 & mask)) ? 1 : 0;
    }
  return c;
}

void Manager::train(Gene &g)
{
  std::vector<unsigned int> fullResults;
  std::vector<unsigned int> bitResults;
  std::vector<unsigned int> output(255);

  _network.load(g);
  for (char c('A') ; c <= 'Z' ; ++c)
    {
      fullResults.push_back(0);
      bitResults.push_back(0);
      if (_dataset[c - 'A'])
	{
	  for (unsigned int fontId(0) ; fontId < _fontsCount ; ++fontId)
	    {
	      loadImage(c, fontId);
	      unsigned char r = _network.getOutput();
	      output[r]++;
	      bitResults[c - 'A'] += getMatchingBits(c, r);
	      fullResults[c- 'A'] += (c == r) ? 1 : 0;
	    }
	}
    }
  g.setFullResults(fullResults);
  g.setBitResults(bitResults);
  g.setOutput(output);
  std::cout << g.printResults(_dataset, _fontsCount, true) << std::endl;
}

void Manager::startTrain()
{
  _train = true;
  unsigned int iteration(0);
  while (_train && !_evolver.getBestGene().perfect(_dataset, _fontsCount))
    {
      if (_evolver.getLastIncrease() && _evolver.getLastIncrease() % 10000 == 0)
	addNeuron(1, 1);
      std::cout << "Iteration " << ++iteration << std::endl;
      for (unsigned int i(0) ; i < 5 ; ++i)
	train(_evolver.getGene(i));
      _evolver.evolve(iteration);
      
      if (_evolver.getBestGene().perfect(_dataset, _fontsCount))
	{
	  std::cout << "PERFECT" << std::endl;
	  if (_fontsCount < 105 && std::count(_dataset.begin(), _dataset.end(), true) > _fontsCount / 2)
	    _fontsCount++;
	  else
	    {
	      for (unsigned int i(0) ; i < _dataset.size() ; ++i)
		if (!_dataset[i])
		  {
		    _dataset[i] = true;
		    break;
		  }
	    }
	}
    }
}

void Manager::stopTrain()
{
  _train = false;
}

void Manager::addNeuron(unsigned int layer, unsigned int number)
{
  _network.load(_evolver.getBestGene());
  for (unsigned int i(0) ; i < number ; ++i)
    _network.addNeuron(layer);
  _network.save(_evolver.getBestGene());
  for (int i(0) ; i < 5 ; ++i)
    _network.save(_evolver.getGene(i));
}

Evolver &Manager::getEvolver()
{
  return _evolver;
}

Network &Manager::getNetwork()
{
  return _network;
}

std::vector<bool> &Manager::getDataset()
{
  return _dataset;
}

unsigned int Manager::getFontCount() const
{
  return _fontsCount;
}

void Manager::setFontCount(unsigned int c)
{
  _fontsCount = c;
}
