#ifndef CRITERIONFACTORY_H
#define CRITERIONFACTORY_H

#include <dlfcn.h>
#include <string.h>
#include <algorithm>
#include <vector>

#include "common.h"
#include "criterion.h"
#include "singleton.h"

class CriterionFactory : public Singleton<CriterionFactory>
{
  friend class Singleton<CriterionFactory>;
private:
  CriterionFactory();
  ~CriterionFactory();
  CriterionFactory(const CriterionFactory &) { }
  CriterionFactory& operator=(const CriterionFactory &) { return *this; }
  std::string criterionDirectory;
public:
  void setCriterionDirectory(std::string path);
  Criterion* open(const char* criterionName);
  void close(struct Criterion* criterion);
};

#endif
