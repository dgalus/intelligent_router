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
	FILE *fp;
	char linec[1000];
	fp = popen("ping -c 2 8.8.4.4", "r");
	if(fp == NULL)
	{
		return -1;
	}
	float time = -1, x;
	while(fgets(linec, sizeof(linec)-1, fp) != NULL)
	{
		std::string line(linec);
		if(line.find("rtt min/avg/max/mdev") != std::string::npos)
		{
			sscanf(line.c_str(), "rtt min/avg/max/mdev = %f/%f/%f/%f ms", &x, &time, &x, &x);
		}
	}
	pclose(fp);
  int val = time/3;
  if(val > 100)
  {
    val = 100;
  }
	return val;
}
