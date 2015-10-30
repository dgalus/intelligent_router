#include <stdio.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include <cmath>
#include <map>
#include <string>

extern "C" std::map<int, std::string> getLevels()
{
  std::map<int, std::string> ret;
  return ret;
}

extern "C" int getMaximumPid()
{
  int maxPid;
  FILE *fp;
	char linec[1000];
	fp = popen("cat /proc/sys/kernel/pid_max", "r");
	if(fp == NULL)
	{
		return -1;
	}
	while(fgets(linec, sizeof(linec)-1, fp) != NULL)
	{
		std::string line(linec);
		sscanf(line.c_str(), "%d", &maxPid);
	}
	pclose(fp);
  return maxPid;
}

extern "C" int calculate()
{
  struct sysinfo si;
  sysinfo(&si);

  int processCount = si.procs;
  int maxPid = getMaximumPid();
  double procUsage = processCount*100.0/maxPid;
  return (int) round(procUsage);
}
