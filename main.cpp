#include <cstdlib> // srand()
#include <iostream>
#include "Manager.h"
#include "Prompt.h"
#include "OpenCL.h"

int main(int ac, char **av)
{
  srand(time(NULL));
  OpenCL cl;
  Manager m;
  Prompt p(m);
  p.run();

  return 0;
}
