#ifndef POLICYFACTORY_H
#define POLICYFACTORY_H

#include <dlfcn.h>
#include <string.h>
#include <algorithm>
#include <map>
#include <string>
#include <vector>

#include "common.h"
#include "policy.h"
#include "singleton.h"

class PolicyFactory : public Singleton<PolicyFactory>
{
  friend class Singleton<PolicyFactory>;
private:
  PolicyFactory();
  ~PolicyFactory();
  PolicyFactory(const PolicyFactory &) { }
  PolicyFactory& operator=(const PolicyFactory &) { return *this; }
  std::string policyDirectory;
public:
  void setPolicyDirectory(std::string & path);
  Policy* open(const char* policyName);
  void close(struct Policy* policy);
};

#endif
