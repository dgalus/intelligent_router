#ifndef POLICY_H
#define POLICY_H

struct Policy
{
  void* handle;
  const char* name;
  const char* command;
  int aggressivnessLevel;
};

#endif
