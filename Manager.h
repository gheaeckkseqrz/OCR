#ifndef __MANAGER_H__
#define __MANAGER_H__

#include <opencv2/opencv.hpp>
#include "Gene.h"
#include "Network.h"

class Manager
{
 public:
  Manager();

  void loadImage(char c, unsigned int fontId);
  float getPixelValue(unsigned int x, unsigned int y) const;

  unsigned int getMatchingBits(char c1, char c2) const;
  void train(Network &n, Gene &g);

 private:
  cv::Mat _image;
};

#endif
