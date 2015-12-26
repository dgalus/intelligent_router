#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../src/configuration.h"

static int aggressivnessLevel = 0;

extern "C" const char* getName()
{
  const char* name = "Policy_blank";
  return name;
}

extern "C" const char* getCommand()
{
char* command = (char *) malloc(strlen(POLICY_DIRECTORY) + strlen("/bin/sh ") + strlen("/policy_blank.sh") + 1);
  memset(command, 0, strlen(POLICY_DIRECTORY) + strlen("/bin/sh ") + strlen("/policy_blank.sh") + 1);
  sprintf(command, "/bin/sh %s/policy_blank.sh", POLICY_DIRECTORY);
  return command;
}

extern "C" int getAggressivnessLevel()
{
  return aggressivnessLevel;
}
