#ifndef __MANAGER_H__
#define __MANAGER_H__

#include <fstream>
#include <opencv2/opencv.hpp>
#include "Gene.h"
#include "Network.h"

class Manager
{
 public:
  Manager();

  void loadImage(char c, unsigned int fontId);
  float getPixelValue(unsigned int x, unsigned int y) const;
  void displayInput();
  std::string neuronsInfo() const;
  std::string neuronInfo(unsigned int neuronId) const;

  unsigned int getMatchingBits(char c1, char c2) const;

  void train(Gene &g);
  bool evolveAndTrain(Gene & g, unsigned int layer = 1);
  void startTrain();
  void stopTrain();
  void cleanNeuron(unsigned int neuronId);
  void cleanAll();
  
  void addNeuron(unsigned int layer, unsigned int number = 1);
  void registerLiveUpdate(unsigned int neuronId, std::string const &tty);
  void liveUpdate();

  Network &getNetwork();
  std::vector<unsigned int> &getDataset();
  
 private:
  Network _network;
  Gene _gene;
  bool _train;
  std::vector<unsigned int> _dataset;
  cv::Mat _image;
  std::vector<std::pair<unsigned int, std::ofstream>> _ttys;
};

#endif
