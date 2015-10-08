#ifndef CRITERION_H
#define CRITERION_H

#include <map>
#include <string>

typedef void (*ScriptFunction)(void);
typedef std::map<std::string, ScriptFunction> FunctionsMap;

struct Criterion
{
  void* handle;
  const char* name;
  std::map<int, std::string> levels;
  int (*calculate)();
  FunctionsMap functions;
};

#endif
