#include <iomanip>
#include "Manager.h"
#include "Network.h"
#include "InputNeuron.h"

Manager::Manager()
  :_network(*this), _train(false), _dataset(26)
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
  std::cout << "Loaded " << c << fontId << std::endl;
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

void Manager::savefeatures()
{
  cv::Mat m(Network::PICTURE_RESOLUTION, Network::PICTURE_RESOLUTION, CV_8UC3);
  std::cout << "Image type is " << _image.type() << std::endl;
  for (auto n : _network._neurons[1])
    {
      for (unsigned int x(0) ; x < Network::PICTURE_RESOLUTION ; ++x)
	for (unsigned int y(0) ; y < Network::PICTURE_RESOLUTION ; ++y)
	  {
	    auto &p = m.at<cv::Vec3b>(cv::Point(x,y));
	    p = cv::Vec3b(0, 0, 0);
	    if (n->_inputs[y * Network::PICTURE_RESOLUTION + x].second > 0)
	      p[1] = (unsigned char)(10.0 / n->_inputs[y * Network::PICTURE_RESOLUTION + x].second * 255);
	    if (n->_inputs[y * Network::PICTURE_RESOLUTION + x].second < 0)
	      p[0] = (unsigned char)(10.0 / n->_inputs[y * Network::PICTURE_RESOLUTION + x].second * 255);

	  }
      std::string path = std::string("./features/") + std::to_string(n->_id) + ".png";
      imwrite(path.c_str(), m);
    }
}

std::string Manager::neuronsInfo() const
{
  std::string result;
  for (unsigned int i(_network.getLayerDescription()[0]) ; i < _network.getNeuronsCount() ; ++i)
    result += neuronInfo(i) + "\n";
  return result;
}

std::string Manager::neuronInfo(unsigned int neuronId) const
{
  std::stringstream ss;
  Neuron *n = _network.getNeuron(neuronId);
  ss << *n;
  return ss.str();
}

void Manager::train()
{

  static int r(0);
  r++;

  loadImage(r % 2 ? 'A' : 'B', std::rand() % 100);
    
  auto o = _network.getOutput();
  std::map<std::string, float> expected;
  expected["Output"] = 0;
  expected["Output"] += (r % 2 ? .95 : .05) - o["Output"];
  _network.adjustWeights(expected);

//    ((InputNeuron*)(_network._neurons[0][0]))->_retvalue = 0;
//    o = _network.getOutput();
//    expected["Output"][0] = (.222 - o["Output"]);
//    _network.adjustWeights(expected);

  return;

  for (char c('A') ; c <= 'Z' ; ++c)
    {
      for (unsigned int fontId(0) ; fontId < _dataset[c - 'A'] ; ++fontId)
	{
	  if (_dataset[c - 'A'])
	    {
	      loadImage(c, fontId);
	      std::cout << "Feeding " << c << fontId << std::endl;
	      auto result = _network.getOutput();

	      std::cout << "Output : ";
	      for (auto a : result)
		{
		  if (a.first == std::string("") + c && a.second > .7)
		    std::cout << "\033[0;32m";
		  if (a.first != std::string("") + c && a.second > .3)
		    std::cout << "\033[0;31m";
		  std::cout << "[" << a.first << "|" << std::setprecision(2) << (a.second > 0.01 ? a.second : 0) << "]";
		  std::cout << "\033[0;00m";
		}
	      std::cout << std::endl;

	      std::map<std::string, std::vector<float>> data;
	      float cost(0);
	      for (char c1('A') ; c1 <= 'Z' ; ++c1)
		{
		  data[std::string("") + c1].push_back(((c == c1) ? 1.0 : 0.0) - result[std::string("") + c1]);
		  cost += fabs(((c == c1) ? 1.0 : 0.0) - result[std::string("") + c1]);
		}

	      //	      std::cout << "Expected : ";
	      //	      for (auto a : data)
	      //std::cout << "[" << a.first << "|" << std::setprecision(2) << (a.second[0] > 0.01 ? a.second[0] : 0) << "]";
	      std::cout << " => " << cost << std::endl;
	      std::cout << std::endl;

	      //	      _network.adjustWeights(data);
	    }
	}
    }
}

void Manager::startTrain()
{
  _train = true;
  while (_train)
    {
      train();
    }
}

void Manager::stopTrain()
{
  _train = false;
}

std::vector<float> Manager::expectedOutput(unsigned char c)
{
  std::vector<float> result;
  unsigned char v(1);
  v = v << 7;
  for (unsigned int i(0) ; i < 8 ; ++i)
    {
      result.push_back((c & v) ? 1.0 : 0.0);
      v = v >> 1;
    }
  return result;
}

void Manager::addNeuron(unsigned int layer, unsigned int number)
{
  for (unsigned int i(0) ; i < number ; ++i)
    _network.addNeuron(layer);
}

void Manager::liveUpdate()
{
  for (auto &tty : _ttys)
    {
      tty.second << "\033[2J\033[1;1H" << neuronInfo(tty.first) << std::endl;
      tty.second << "Neuron : " << tty.first << std::endl;
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

std::vector<unsigned int> &Manager::getDataset()
{
  return _dataset;
}
