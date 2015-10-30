#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <string>

extern "C" std::map<int, std::string> getLevels()
{
  std::map<int, std::string> ret;
  return ret;
}

extern "C" int calculate()
{

}
