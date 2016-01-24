#ifndef __MANAGER_H__
#define __MANAGER_H__

#include <fstream>
#include <opencv2/opencv.hpp>
#include "Gene.h"
#include "Network.h"
#include "Evolver.h"

class Manager
{
 public:
  Manager();

  void loadImage(char c, unsigned int fontId);
  float getPixelValue(unsigned int x, unsigned int y) const;
  void displayInput();
  std::string neuronInfo(unsigned int neuronId) const;

  unsigned int getMatchingBits(char c1, char c2) const;

  void train(Gene &g);
  void startTrain();
  void stopTrain();
  void cleanNeuron(unsigned int neuronId);
  void cleanAll();
  
  void addNeuron(unsigned int layer, unsigned int number = 1);
  void registerLiveUpdate(unsigned int neuronId, std::string const &tty);
  void liveUpdate();

  Evolver &getEvolver();
  Network &getNetwork();
  std::vector<bool> &getDataset();
  unsigned int getFontCount() const;
  void setFontCount(unsigned int c);
  
 private:
  Network _network;
  bool _train;
  Evolver _evolver;
  std::vector<bool> _dataset;
  unsigned int _fontsCount;
  cv::Mat _image;
  std::vector<std::pair<unsigned int, std::ofstream>> _ttys;
};

#endif
