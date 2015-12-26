#include <stdio.h>

static int aggressivnessLevel = 30;

extern "C" const char* getName()
{
  const char* name = "Policy30";
  return name;
}

extern "C" const char* getCommand()
{
  char* command = (char *) malloc(strlen(POLICY_DIRECTORY) + strlen("/bin/sh ") + strlen("/policy30.sh") + 1);
  memset(command, 0, strlen(POLICY_DIRECTORY) + strlen("/bin/sh ") + strlen("/policy30.sh") + 1);
  sprintf(command, "/bin/sh %s/policy30.sh", POLICY_DIRECTORY);
  return command;
}

extern "C" int getAggressivnessLevel()
{
  return aggressivnessLevel;
}
