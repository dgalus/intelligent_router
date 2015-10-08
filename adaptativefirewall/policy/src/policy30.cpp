#include <stdio.h>
#include <stdlib.h>
#include <string>

static std::string name = "Policy30";
static std::string command = "bash ../policy/policy30.sh";
static int aggressivnessLevel = 30;

extern "C" std::string getName()
{
  return name;
}

extern "C" std::string getCommand()
{
  return command;
}

extern "C" int getAggressivnessLevel()
{
  return aggressivnessLevel;
}
