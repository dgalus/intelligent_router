#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <fstream>
#include <limits>

extern "C" std::map<int, std::string> getLevels()
{
  std::map<int, std::string> ret;
  return ret;
}

extern "C" unsigned long getMemTotal()
{
  std::string token;
  std::ifstream file("/proc/meminfo");
  while(file >> token)
  {
    if(token == "MemTotal:")
    {
      unsigned long mem;
      if(file >> mem)
      {
        return mem/1024;
      }
      else
      {
        return 0;
      }
    }
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
  return 0;
}

extern "C" int calculate()
{
  FILE *fp;
  char linec[1000];
  fp = popen("free -m | grep ^-/+ | tr -s ' ' | cut -d ' ' -f 4", "r");
  if(fp == NULL)
  {
    return -1;
  }
  int bufferedRam;
  while(fgets(linec, sizeof(linec)-1, fp) != NULL)
  {
    sscanf(linec, "%d", &bufferedRam);
  }
  pclose(fp);
  unsigned long totalRam = getMemTotal();
  unsigned long freeRam = totalRam - bufferedRam;
  double usage = freeRam*100.0/totalRam;
  return (int) usage;
}
