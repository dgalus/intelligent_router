#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <map>
#include <string>

extern "C" std::map<int, std::string> getLevels()
{
  std::map<int, std::string> ret;
  ret.insert(std::pair<int, std::string>(70, "level70"));
  return ret;
}

extern "C" int calculate()
{
  FILE *fp;
  char linec[1000];
  long values1[7];
  fp = fopen("/proc/stat", "r");
  if(fp == NULL)
  {
    return -1;
  }
  fgets(linec, sizeof(linec)-1, fp);
  sscanf(linec, "cpu %ld %ld %ld %ld %ld %ld %ld", &values1[0], &values1[1], &values1[2], &values1[3], &values1[4], &values1[5], &values1[6]);
  long total_jiffies_1 = values1[0] + values1[1] + values1[2] + values1[3] + values1[4] + values1[5] + values1[6];
  long work_jiffies_1 = values1[0] + values1[1] + values1[2];
  fclose(fp);

  sleep(2);

  fp = fopen("/proc/stat", "r");
  if(fp == NULL)
  {
    return -1;
  }
  fgets(linec, sizeof(linec)-1, fp);
  sscanf(linec, "cpu %ld %ld %ld %ld %ld %ld %ld", &values1[0], &values1[1], &values1[2], &values1[3], &values1[4], &values1[5], &values1[6]);
  long total_jiffies_2 = values1[0] + values1[1] + values1[2] + values1[3] + values1[4] + values1[5] + values1[6];
  long work_jiffies_2 = values1[0] + values1[1] + values1[2];
  fclose(fp);

  long work_over_period = work_jiffies_2 - work_jiffies_1;
  long total_over_period = total_jiffies_2 - total_jiffies_1;

  return (int) work_over_period*100./total_over_period;
}

extern "C" void level70()
{
  std::string str = "kill `ps -e -o pid,%cpu,comm= | sort -n -k 2 -r | awk {'print $1; exit'}`";
  system(str.c_str());
}
