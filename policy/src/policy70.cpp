#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../src/configuration.h"

static int aggressivnessLevel = 70;

extern "C" const char* getName()
{
  const char* name = "Policy70";
  return name;
}

extern "C" const char* getCommand()
{
  char* command = (char *) malloc(strlen(POLICY_DIRECTORY) + strlen("/bin/sh ") + strlen("/policy70.sh") + 1);
  memset(command, 0, strlen(POLICY_DIRECTORY) + strlen("/bin/sh ") + strlen("/policy70.sh") + 1);
  sprintf(command, "/bin/sh %s/policy70.sh", POLICY_DIRECTORY);
  return command;
}

extern "C" int getAggressivnessLevel()
{
  return aggressivnessLevel;
}
