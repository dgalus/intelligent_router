#include <stdio.h>
#include <stdlib.h>
#include <string>

static int aggressivnessLevel = 30;

extern "C" char* getName()
{
  char* name = "Policy30";
  return name;
}

extern "C" char* getCommand()
{
  char* command = "bash ../policy/policy30.sh";
  return command;
}

extern "C" int getAggressivnessLevel()
{
  return aggressivnessLevel;
}
