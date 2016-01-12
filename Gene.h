#ifndef __GENE_H__
#define __GENE_H__

#include <vector>

class Gene
{
 public:
  Gene();

  void clear();
  void addWeight(float w);
  float getWeight(unsigned int index) const;

  void mute(unsigned int probability = 5);
  void mute(Gene const &model, unsigned int probability = 5);

  void setFullResults(std::vector<unsigned int> const &results);
  void setBitResults(std::vector<unsigned int> const &results);
  void setOutput(std::vector<unsigned int> const &output);
  std::string printResults(bool color = false) const;
  float getScore() const;

  void saveToFile(std::string const &path);

 private:
  std::vector<float> _weights;
  std::vector<unsigned int> _fullResults;
  std::vector<unsigned int> _bitResults;
  std::vector<unsigned int> _output;
};

#endif
