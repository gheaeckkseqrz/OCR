#include <iomanip>
#include "Manager.h"
#include "Network.h"

Manager::Manager()
  :_network(*this), _train(false), _dataset(26), _fontsCount(0)
{
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

std::string Manager::neuronInfo(unsigned int neuronId) const
{
  std::stringstream ss;
  Neuron *n = _network.getNeuron(neuronId);
  if (n)
    {
      Gene g;
      n->saveWeights(g);

      for (unsigned int i(0) ; i < g._weights.size() ; ++i)
	{
	  if (i && i % Network::PICTURE_RESOLUTION == 0)
	    ss << std::endl;
	  if (g._weights[i] > -0.1 && g._weights[i] < 0.1)
	    ss << "[\033[0;34m" << std::fixed << std::setprecision(1) << g._weights[i] * ( g._weights[i] >= 0 ? 1 : -1) << "\033[0;00m]";
	  else if (g._weights[i] > 0)
	    ss << "[\033[0;32m" << std::fixed << std::setprecision(1) << g._weights[i] << "\033[0;00m]";
	  else
	    ss << "[\033[0;31m" << std::fixed << std::setprecision(1) << g._weights[i] * -1 << "\033[0;00m]";
	}
      ss << std::endl;
    }
  return ss.str();
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
}

bool Manager::evolveAndTrain(Gene &g, unsigned int layer)
{
  unsigned int start(0);
  unsigned int end(0);
  for (unsigned int l(0) ; l < layer ; ++l)
    start += _network.getSynapsesCount(l);
  end = start;
  start -= _network.getNeuronsCount(layer - 1);

  for (unsigned int n(0) ; n < _network.getNeuronsCount(layer) ; ++n)
    {
      Gene g2 = g;
      start += _network.getNeuronsCount(layer - 1);
      end += _network.getNeuronsCount(layer - 1);

      while (g.compare(g2, start, end))
	g2.mute(10, start, end);

      train(g2);
      if (g2.getScore() > _gene.getScore())
	{
	  g = g2;
	  return true;
	}
      if (_network.getNeuronsCount(layer + 1))
	{
	  for (unsigned int i(0) ; i < 10 ; ++i)
	    if (evolveAndTrain(g2, layer + 1))
	      {
		g = g2;
		return true;
	      }
	}
    }
  return false;
}

void Manager::startTrain()
{
  _train = true;
  unsigned int iteration(0);
  unsigned int lastIncrease(0);
  if (_gene._weights.size() != _network.getSynapsesCount())
    _network.save(_gene);
  while (_train && !_gene.perfect(_dataset, _fontsCount))
    {
      lastIncrease++;
      if (lastIncrease && lastIncrease % 25000 == 0)
      	addNeuron(1, 1);
      std::cout << "Iteration " << ++iteration << std::endl;
      Gene g = _gene;
      if (evolveAndTrain(g, 1))
	{
	  _gene = g;
 	  cleanAll();
 	  liveUpdate();
	  lastIncrease = 0;
	}
      float diff = g.getScore() - _gene.getScore();
      std::cout << g.printResults(_dataset, _fontsCount, true) << " - (" << (diff > 0 ? "\033[0;32m" : "\033[0;31m") << diff << "\033[0;00m) - LastIncrease : " << lastIncrease << std::endl;
      if (_gene.perfect(_dataset, _fontsCount))
	{
	  std::cout << "PERFECT" << std::endl;
	  if (_fontsCount < 105 && std::count(_dataset.begin(), _dataset.end(), true) > _fontsCount / 2)
	    _fontsCount++;
	  else
	    {
	      while (std::count(_dataset.begin(), _dataset.end(), true) != _dataset.size())
		{
		  unsigned int randomId = std::rand() % _dataset.size();
		  if (!_dataset[randomId])
		    {
		      _dataset[randomId] = true;
		      break;
		    }
		}
	    }
	}
    }
}

void Manager::stopTrain()
{
  _train = false;
}

void Manager::cleanNeuron(unsigned int neuronId)
{
  _network.load(_gene);
  float best = _gene.getScore();
  Neuron *n = _network.getNeuron(neuronId);
  if (n)
    {
      Gene cleanGene;
      Gene trainGene;
      n->saveWeights(cleanGene);
      unsigned int i(0);
      for (auto &w : cleanGene._weights)
	{
	  if (w != 0)
	    { 
	      float backup = w;
	      w = 0.0;
	      n->loadWeights(cleanGene, 0);
	      _network.save(trainGene);
	      train(trainGene);
	      if (trainGene.getScore() < best)
		w = backup;
	    }
	}
      n->loadWeights(cleanGene, 0);
      _network.save(_gene);
      train(_gene);
      liveUpdate();
    }
}

void Manager::cleanAll()
{
  for (unsigned int i(std::pow(Network::PICTURE_RESOLUTION, 2) - 1) ; i < _network.getNeuronsCount() ; ++i)
    {
      std::cout << "\rCleaning " << i << " / " << _network.getNeuronsCount();
      std::cout.flush();
      cleanNeuron(i);
    }
  std::cout << std::endl;
}

void Manager::addNeuron(unsigned int layer, unsigned int number)
{
  _network.load(_gene);
  for (unsigned int i(0) ; i < number ; ++i)
    _network.addNeuron(layer);
  _network.save(_gene);
  train(_gene); // Re-save score
}

void Manager::liveUpdate()
{
  _network.load(_gene);
  for (auto &tty : _ttys)
    {
      tty.second << "\033[2J\033[1;1H" << neuronInfo(tty.first) << std::endl;
      tty.second << "Neuron : " << tty.first << std::endl;
      tty.second << _gene.printResults(_dataset, _fontsCount, true) << std::endl;
    }
}

void Manager::registerLiveUpdate(unsigned int neuronId, std::string const &tty)
{
  _ttys.push_back(std::pair<unsigned int, std::ofstream>(neuronId, std::ofstream(tty)));
  liveUpdate();
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
