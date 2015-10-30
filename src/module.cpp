#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <map>

#include "server.h"

char* moduleDir;

struct ServerModule* moduleOpen(const char *moduleName)
{
    char* modulePath;
    void* handle;
    void (* moduleGenerate)(int, std::map<std::string, std::string>);
    struct ServerModule* module;

    modulePath = (char *) xmalloc(strlen(moduleDir) + strlen(moduleName) + 2);
    sprintf(modulePath, "%s/%s", moduleDir, moduleName);
    handle = dlopen(modulePath, RTLD_LAZY);
    free(modulePath);
    if(handle == NULL)
    {
        return NULL;
    }
    moduleGenerate = (void (*) (int, std::map<std::string, std::string>)) dlsym(handle, "moduleGenerate");
    if(moduleGenerate == NULL)
    {
        dlclose(handle);
        return NULL;
    }
    module = (struct ServerModule *) xmalloc(sizeof(struct ServerModule));
    module->handle = handle;
    module->name = xstrdup(moduleName);
    module->generateFunction = moduleGenerate;

    return module;
}

void moduleClose(struct ServerModule *module)
{
    dlclose(module->handle);
    free((char *) module->name);
    free(module);
}
