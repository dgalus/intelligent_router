#include "criterionfactory.h"

CriterionFactory::CriterionFactory()
{ }

CriterionFactory::~CriterionFactory()
{ }

void CriterionFactory::setCriterionDirectory(std::string path)
{
  this->criterionDirectory = path;
}

Criterion* CriterionFactory::open(const char* criterionName)
{
    char* criterionPath;
    void* handle;
    int (* calculate)();
    std::map<int, std::string> (*getLevels)();

    criterionPath = (char *) xmalloc(this->criterionDirectory.length() + strlen(criterionName) + 2);
    sprintf(criterionPath, "%s/%s", this->criterionDirectory.c_str(), criterionName);

    handle = dlopen(criterionPath, RTLD_NOW);
    free(criterionPath);
    if(handle == NULL)
    {
        return NULL;
    }

    calculate = (int (*) ()) dlsym(handle, "calculate");
    if(calculate == NULL)
    {
        dlclose(handle);
        return NULL;
    }

    getLevels = (std::map<int, std::string> (*)()) dlsym(handle, "getLevels");
    if(getLevels == NULL)
    {
        dlclose(handle);
        return NULL;
    }

    std::map<int, std::string> levels = getLevels();
    std::vector<std::string> functionNames;
    for(auto it = levels.begin(); it != levels.end(); it++)
    {
        if(!(it->second.empty()))
        {
            functionNames.push_back(it->second);
        }
    }

    FunctionsMap fm;
    for(auto it = functionNames.begin(); it != functionNames.end(); it++)
    {
        void (* func)();
        func = (void (*)()) dlsym(handle, (*it).c_str());
        if(func == NULL)
        {
            dlclose(handle);
            return NULL;
        }
        fm.insert(std::pair<std::string, ScriptFunction>((*it), func));
    }
    Criterion* criterion;
    criterion = (struct Criterion*) xmalloc(sizeof(struct Criterion));

    criterion->calculate = calculate;
    criterion->handle = handle;
    criterion->levels = getLevels();
    criterion->name = xstrdup(criterionName);
    criterion->functions = fm;

    return criterion;
}

void CriterionFactory::close(struct Criterion* criterion)
{
    dlclose(criterion->handle);
    free(criterion);
}
