#include <stdio.h>

static int aggressivnessLevel = 30;

extern "C" const char* getName()
{
  const char* name = "Policy30";
  return name;
}

extern "C" const char* getCommand()
{
  const char* command = "/bin/sh ../policy/policy30.sh";
  return command;
}

extern "C" int getAggressivnessLevel()
{
  return aggressivnessLevel;
}
