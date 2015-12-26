#include <stdio.h>

static int aggressivnessLevel = 70;

extern "C" const char* getName()
{
  const char* name = "Policy70";
  return name;
}

extern "C" const char* getCommand()
{
  const char* command = "/bin/sh ../policy/policy70.sh";
  return command;
}

extern "C" int getAggressivnessLevel()
{
  return aggressivnessLevel;
}
