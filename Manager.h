#ifndef __MANAGER_H__
#define __MANAGER_H__

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

  unsigned int getMatchingBits(char c1, char c2) const;

  void train(Gene &g);
  void startTrain();
  void stopTrain();
  
  void addNeuron(unsigned int number = 1);

  Evolver &getEvolver();
  Network &getNetwork();

 private:
  Network _network;
  bool _train;
  Evolver _evolver;
  cv::Mat _image;
};

#endif
