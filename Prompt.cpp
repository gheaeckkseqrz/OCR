#include <iomanip>
#include <iostream>
#include <thread>
#include "Prompt.h"

Prompt::Prompt(Manager &manager)
  :_manager(manager)
{

}

void Prompt::run()
{
  while (true)
    {
      std::string input;
      std::cout << ">> ";
      std::getline(std::cin, input);

      if (input == "quit" || input == "break")
	break;
      if (input == "train")
	train();
      if (input == "result")
	std::cout << _manager.getEvolver().getBestGene().printResults(_manager.getDataset(), true) << std::endl;
      if (input == "add")
	add();
      if (input == "description")
	std::cout << _manager.getNetwork().getDescription() << std::endl;
      if (input == "layerdescription")
	layerDescription();
      if (input == "dataset")
	dataset();
      if (input == "neuroninfo")
	neuronInfo();
      if (input == "reset")
	_manager.getEvolver().forgetBestScore();
      if (input == "displayinput")
	_manager.displayInput();
      if (input == "loadimage")
	loadImage();
    }
}

void Prompt::train()
{
  std::string s;
  std::thread t = std::thread(&Manager::startTrain, &_manager);
  std::getline(std::cin, s);
  _manager.stopTrain();
  t.join();  
}

void Prompt::add(unsigned int layer, unsigned int count)
{
  unsigned int l(layer);
  unsigned int c(count);
  if (l == -1)
    {
      std::cout << "Layer : ";
      std::cin >> l;
    }
  if (c == -1)
    {
      std::cout << "Count : ";
      std::cin >> c;
    }
  _manager.addNeuron(l, c);
}

void Prompt::dataset()
{
  unsigned int fontCount;
  std::vector<bool> &dataset = _manager.getDataset();
  for (char c('A') ; c <= 'Z' ; ++c)
    std::cout << "[" << (dataset[c - 'A'] ? c : ' ') << "]";
  std::cout << " (" << _manager.getFontCount() << ")" << std::endl;
  std::string input;
  std::cout << "To add : ";
  std::getline(std::cin, input);
  for (auto c : input)
    if (c >= 'A' && c <= 'Z')
      dataset[c - 'A'] = true;
  std::cout << "To remove : ";
  std::getline(std::cin, input);
  for (auto c : input)
    if (c >= 'A' && c <= 'Z')
      dataset[c - 'A'] = false;
  std::cout << "Font Count : ";
  std::cin >> fontCount;
  _manager.setFontCount(fontCount);
  for (char c('A') ; c <= 'Z' ; ++c)
    std::cout << "[" << (dataset[c - 'A'] ? c : ' ') << "]";
  std::cout << " (" << _manager.getFontCount() << ")" << std::endl;
}

void Prompt::layerDescription()
{
  unsigned int layer;
  std::cout << "Layer : ";
  std::cin >> layer;
  std::cout << _manager.getNetwork().getDescription(layer) << std::endl;
}

void Prompt::neuronInfo()
{
  unsigned int neuronId(0);
  std::cout << "Neuron Id : ";
  std::cin >> neuronId;

  Neuron *n = _manager.getNetwork().getNeuron(neuronId);
  if (n)
    {
      Gene g;
      n->saveWeights(g);

      for (unsigned int i(0) ; i < g._weights.size() ; ++i)
	{
	  if (i && i % Network::PICTURE_RESOLUTION == 0)
	    std::cout << std::endl;
	  if (g._weights[i] > -0.1 && g._weights[i] < 0.1)
	    std::cout << "[\033[0;34m" << std::fixed << std::setprecision(1) << g._weights[i] * ( g._weights[i] >= 0 ? 1 : -1) << "\033[0;00m]";
	  else if (g._weights[i] > 0)
	    std::cout << "[\033[0;32m" << std::fixed << std::setprecision(1) << g._weights[i] << "\033[0;00m]";
	  else
	    std::cout << "[\033[0;31m" << std::fixed << std::setprecision(1) << g._weights[i] * -1 << "\033[0;00m]";
	}
      std::cout << std::endl;
    }
}

void Prompt::loadImage()
{
  char c;
  unsigned int fontId;

  std::cout << "Char : ";
  std::cin >> c;
  std::cout << "FontId : ";
  std::cin >> fontId;

  _manager.loadImage(c, fontId);
}
