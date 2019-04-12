#include "registrar.h"
#include <iostream>

int main(int argc, char* argv[])
{
  Simulation s;
  string location = "";
  if(argc == 2)
  {
    location = argv[1];
  }
  s.readFile(location);
  //s.outputArray();
  s.selectPos();
  //s.times();
  s.run();
  return 0;
}
