#ifndef POLICY_H
#define POLICY_H

struct Policy
{
  void* handle;
  std::string name;
  std::string command;
  int aggressivnessLevel;
};

#endif
