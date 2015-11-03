#include <stdio.h>
#include <stdlib.h>
#include <string>

static int aggressivnessLevel = 30;

extern "C" const char* getName()
{
  const char* name = "Policy30";
  return name;
}

extern "C" const char* getCommand()
{
  const char* command = "bash ../policy/policy30.sh";
  return command;
}

extern "C" int getAggressivnessLevel()
{
  return aggressivnessLevel;
}
