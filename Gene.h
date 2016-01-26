#ifndef __GENE_H__
#define __GENE_H__

#include <vector>

class Gene
{
 public:
  Gene(std::string const &filename = "");

  void loadFromFile(std::string const &path);
  void clear();
  void addWeight(float w);
  float getWeight(unsigned int index) const;

  void mute(unsigned int probability = 5, unsigned int start = 0, unsigned int end = 0);
  void mute(Gene const &model, unsigned int probability = 5, unsigned int start = 0, unsigned int end = 0);

  void setFullResults(std::vector<unsigned int> const &results);
  void setBitResults(std::vector<unsigned int> const &results);
  void setOutput(std::vector<unsigned int> const &output);
  std::string printResults(std::vector<bool> const &dataset, unsigned int fontsCount, bool color = false) const;
  float getScore() const;
  float getWeightsSum() const;
  bool perfect(std::vector<bool> const &dataset, unsigned int fontsCount) const;
  bool compare(Gene const &o, unsigned int start = 0, unsigned int end = -1) const;

  void saveToFile(std::string const &path);

  std::vector<float> _weights;

 private:
  std::vector<unsigned int> _fullResults;
  std::vector<unsigned int> _bitResults;
  std::vector<unsigned int> _output;
};

#endif
