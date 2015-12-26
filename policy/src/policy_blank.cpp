#include <stdio.h>

static int aggressivnessLevel = 0;

extern "C" const char* getName()
{
  const char* name = "Policy_blank";
  return name;
}

extern "C" const char* getCommand()
{
  const char* command = "/bin/sh ../policy/policy_blank.sh";
  return command;
}

extern "C" int getAggressivnessLevel()
{
  return aggressivnessLevel;
}
