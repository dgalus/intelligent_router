#ifndef POLICYAPPLIER_H
#define POLICYAPPLIER_H

#include <map>
#include <string>
#include <vector>

#include <math.h>

#include "adaptivefirewall.h"
#include "criterionfactory.h"
#include "policyfactory.h"

class PolicyApplier
{
public:
  PolicyApplier(std::string criterionDirectory, std::string policyDirectory);
  ~PolicyApplier();
  void loop();

private:
  inline void readCriterionsConfig();
  inline void readPoliciesConfig();
  inline void collectCriterions();
  inline void collectPolicies();
  inline void choosePolicy();
  inline void doCriterionSpecificActions();
  inline void applyPolicy();
  inline void flushCriterions();
  inline void flushPolicies();
  static Criterion* handleCriterion(std::string name);
  static Policy* handlePolicy(std::string name);

  std::string policyDirectory;
  std::string criterionDirectory;
  Policy* selectedPolicy;
  std::vector<Criterion*> criterions;
  std::vector<Policy*> policies;
  std::vector<std::string> loadedRules;
  std::vector<std::string> policiesNames;
  std::vector<std::string> criterionsNames;
  std::map<Criterion*, int> results;
  time_t criterionConfigFileTimestamp;
  time_t policyConfigFileTimestamp;
};

#endif
