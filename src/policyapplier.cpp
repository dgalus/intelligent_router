#include "policyapplier.h"

PolicyApplier::PolicyApplier(std::string criterionDirectory, std::string policyDirectory)
{
  this->criterionDirectory = criterionDirectory;
  this->policyDirectory = policyDirectory;
  CriterionFactory::getInstance()->setCriterionDirectory(criterionDirectory);
  criterionConfigFileTimestamp = FileHelper::getFileCreationTimestamp(CRITERIONS_CONFIG_PATH) - 1;
  PolicyFactory::getInstance()->setPolicyDirectory(policyDirectory);
  policyConfigFileTimestamp = FileHelper::getFileCreationTimestamp(POLICIES_CONFIG_PATH) - 1;
  selectedPolicy = NULL;
}

PolicyApplier::~PolicyApplier()
{
  delete this;
}

void PolicyApplier::loop()
{
  time_t criterionTimestamp = FileHelper::getFileCreationTimestamp(CRITERIONS_CONFIG_PATH);
  if(criterionConfigFileTimestamp != criterionTimestamp)
  {
    flushCriterions();
    criterionConfigFileTimestamp = criterionTimestamp;
    readCriterionsConfig();
    collectCriterions();
  }
  time_t policyTimestamp = FileHelper::getFileCreationTimestamp(POLICIES_CONFIG_PATH);
  if(policyConfigFileTimestamp != policyTimestamp)
  {
    flushPolicies();
    policyConfigFileTimestamp = policyTimestamp;
    readPoliciesConfig();
    collectPolicies();
  }
  choosePolicy();
  doCriterionSpecificActions();
  applyPolicy();
}

void PolicyApplier::readCriterionsConfig()
{
  CriterionsConfiguration *cc = new CriterionsConfiguration();
  cc->setPath(CRITERIONS_CONFIG_PATH);
  criterionsNames = cc->getConfig();
}

void PolicyApplier::readPoliciesConfig()
{
  PoliciesConfiguration *pc = new PoliciesConfiguration();
  pc->setPath(POLICIES_CONFIG_PATH);
  policiesNames = pc->getConfig();
}

void PolicyApplier::collectCriterions()
{
  for(std::vector<std::string>::iterator it = criterionsNames.begin(); it != criterionsNames.end(); it++)
  {
    Criterion* c = PolicyApplier::handleCriterion(*it);
    if(c != NULL)
    {
      criterions.push_back(c);
    }
  }
}

void PolicyApplier::collectPolicies()
{
  for(std::vector<std::string>::iterator it = policiesNames.begin(); it != policiesNames.end(); it++)
  {
    Policy* p = PolicyApplier::handlePolicy(*it);
    if(p != NULL)
    {
      policies.push_back(p);
    }
  }
}

void PolicyApplier::choosePolicy()
{
  std::for_each(criterions.begin(), criterions.end(), [this](Criterion* c){
    results.insert(std::pair<Criterion*, int>(c, c->calculate()));
//    fprintf(stderr, "%s -> %d\n", results.rbegin()->first->name, results.rbegin()->second);
  });

  if(!results.empty())
  {
    int aggressivnessLevel = results.begin()->second;
    for(std::map<Criterion*, int>::iterator it = results.begin(); it != results.end(); it++)
    {
      if(aggressivnessLevel < it->second)
      {
        aggressivnessLevel = it->second;
      }
    }

    if(!policies.empty())
    {
      std::vector<Policy*> policiesToUse;
      for(std::vector<Policy*>::iterator it = policies.begin(); it != policies.end(); it++)
      {
        if((*it)->aggressivnessLevel < aggressivnessLevel)
        {
          policiesToUse.push_back(*it);
        }
      }
      if(!policiesToUse.empty())
      {
        selectedPolicy = policiesToUse.at(0);
        int diff = abs(aggressivnessLevel - policiesToUse.at(0)->aggressivnessLevel);
        std::for_each(policiesToUse.begin(), policiesToUse.end(), [this, &diff, &aggressivnessLevel](Policy* p){
          if(diff > abs(aggressivnessLevel - p->aggressivnessLevel))
          {
            diff = abs(aggressivnessLevel - p->aggressivnessLevel);
            selectedPolicy = p;
          }
        });
      }
    }
  }
}

void PolicyApplier::doCriterionSpecificActions()
{
  for(std::map<Criterion*, int>::iterator it = results.begin(); it != results.end(); it++)
  {
    Criterion* c = it->first;
    if(c->levels.empty())
    {
      continue;
    }
    std::vector<int> levels;
    for(std::map<int, std::string>::iterator it2 = c->levels.begin(); it2 != c->levels.end(); it2++)
    {
      if(it2->first < it->second)
      {
        levels.push_back(it2->first);
      }
    }
    if(!levels.empty())
    {
      int nearest = levels.at(0);
      int diff = abs(it->second - levels.at(0));
      std::for_each(levels.begin(), levels.end(), [&nearest, &diff, &it](int & val){
        if(diff > abs(val - it->second))
        {
          nearest = val;
          diff = abs(val - it->second);
        }
      });
      std::string functionName = c->levels[nearest];
      (c->functions[functionName])();
    }
  }
}

void PolicyApplier::applyPolicy()
{
  if(selectedPolicy != NULL)
  {
    if(system(selectedPolicy->command))
    {
      std::string log = "Failed to apply policy " + std::string(selectedPolicy->name);
      Log::getInstance()->append(log);
    }
    else
    {
      std::string log = "Policy " + std::string(selectedPolicy->name) + " applied";
      Log::getInstance()->append(log);
    }
  }
}

void PolicyApplier::flushCriterions()
{
  criterions.erase(criterions.begin(), criterions.end());
  criterionsNames.erase(criterionsNames.begin(), criterionsNames.end());
}

void PolicyApplier::flushPolicies()
{
  policies.erase(policies.begin(), policies.end());
  policiesNames.erase(policiesNames.begin(), policiesNames.end());
  if(selectedPolicy != NULL)
  {
    free(selectedPolicy);
  }
}

Criterion* PolicyApplier::handleCriterion(std::string name)
{
  name.append(".so");
  return CriterionFactory::getInstance()->open(name.c_str());
}

Policy* PolicyApplier::handlePolicy(std::string name)
{
  name.append(".so");
  return PolicyFactory::getInstance()->open(name.c_str());
}
