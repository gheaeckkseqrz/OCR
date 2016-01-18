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
	std::cout << _manager.getEvolver().getBestGene().printResults(true) << std::endl;
      if (input == "add")
	add();
      if (input == "description")
	std::cout << _manager.getNetwork().getDescription() << std::endl;
    }
}

void Prompt::train()
{
  std::thread t = std::thread(&Manager::startTrain, &_manager);
  getchar();
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
