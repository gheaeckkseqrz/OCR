#ifndef __MANAGER_H__
#define __MANAGER_H__

#include <fstream>
#include <opencv2/opencv.hpp>
#include "Network.h"

class Manager
{
 public:
  Manager();

  void loadImage(char c, unsigned int fontId);
  float getPixelValue(unsigned int x, unsigned int y) const;
  void displayInput();
  void savefeatures();
  std::string neuronsInfo() const;
  std::string neuronInfo(unsigned int neuronId) const;

  void train();
  void startTrain();
  void stopTrain();

  std::vector<float> expectedOutput(unsigned char c);
  
  void addNeuron(unsigned int layer, unsigned int number = 1);
  void registerLiveUpdate(unsigned int neuronId, std::string const &tty);
  void liveUpdate();

  Network &getNetwork();
  std::vector<unsigned int> &getDataset();
  
 private:
  Network _network;
  bool _train;
  std::vector<unsigned int> _dataset;
  cv::Mat _image;
  std::vector<std::pair<unsigned int, std::ofstream>> _ttys;
};

#endif
