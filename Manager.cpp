#include "manager.h"
#include "Network.h"

Manager::Manager()
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
  cv::resize(_image, _image, cv::Size(16, 16));
}

float Manager::getPixelValue(unsigned int x, unsigned int y) const
{
  if (x < _image.rows && y < _image.cols)
    {
      return (float)_image.at<unsigned char>(x, y) / 255;
    }
  std::cerr << "Invalid pixel coordinate [" << x << "/" << y << "]" << std::endl;
  std::cerr << "Image size is [" << _image.rows << "/" << _image.cols << "]" << std::endl;
  return 0;
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

void Manager::train(Network &n, Gene &g)
{
  std::vector<unsigned int> fullResults;
  std::vector<unsigned int> bitResults;
  std::vector<unsigned int> output(255);

  n.load(g);
  for (char c('A') ; c <= 'Z' ; ++c)
    {
      fullResults.push_back(0);
      bitResults.push_back(0);
      for (unsigned int fontId(0) ; fontId < 114 ; ++fontId)
	{
	  loadImage(c, fontId);
	  unsigned char r = n.getOutput();
	  //	  std::cout << "[" << (int)c << "] => [" << (int)r << "]" << std::endl;
	  output[r]++;
	  bitResults[c - 'A'] += getMatchingBits(c, r);
	  fullResults[c- 'A'] += (c == r) ? 1 : 0;
	}
    }
  g.setFullResults(fullResults);
  g.setBitResults(bitResults);
  g.setOutput(output);
  std::cout << g.printResults(true) << std::endl;
}
