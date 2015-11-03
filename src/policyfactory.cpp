#include "policyfactory.h"

PolicyFactory::PolicyFactory()
{ }

PolicyFactory::~PolicyFactory()
{ }

void PolicyFactory::setPolicyDirectory(std::string & path)
{
    this->policyDirectory = path;
}

Policy* PolicyFactory::open(const char *policyName)
{
    char* policyPath;
    void* handle;
    const char* (* getName)();
    const char* (* getCommand)();
    int (* getAggressivnessLevel)();

    policyPath = (char *) xmalloc(this->policyDirectory.length() + strlen(policyName) + 2);
    sprintf(policyPath, "%s/%s", this->policyDirectory.c_str(), policyName);
    handle = dlopen(policyPath, RTLD_NOW);
    free(policyPath);
    if(handle == NULL)
    {
        return NULL;
    }

    getName = (const char* (*)()) dlsym(handle, "getName");
    if(getName == NULL)
    {
        dlclose(handle);
        return NULL;
    }

    getCommand = (const char* (*)()) dlsym(handle, "getCommand");
    if(getCommand == NULL)
    {
        dlclose(handle);
        return NULL;
    }

    getAggressivnessLevel = (int (*)()) dlsym(handle, "getAggressivnessLevel");
    if(getAggressivnessLevel == NULL)
    {
        dlclose(handle);
        return NULL;
    }

    Policy* policy;
    policy = (struct Policy *) xmalloc(sizeof(struct Policy));
    policy->aggressivnessLevel = getAggressivnessLevel();
    policy->command = getCommand();
    policy->handle = handle;
    policy->name = getName();

    return policy;
}

void PolicyFactory::close(Policy *policy)
{
    dlclose(policy->handle);
    free(policy);
}
