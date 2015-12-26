#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <map>

#include "server.h"

const char* moduleDir;

struct ServerModule* moduleOpen(const char *moduleName)
{
    void* handle;
    void (* moduleGenerate)(int, std::map<std::string, std::string>);
    struct ServerModule* module;

    handle = dlopen(moduleName, RTLD_LAZY);
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
