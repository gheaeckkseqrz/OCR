#include <cstdlib> // srand()
#include <iostream>
#include "Manager.h"
#include "Prompt.h"

int main(int ac, char **av)
{
  srand(time(NULL));
  Manager m;
  Prompt p(m);
  p.run();

  return 0;
}
