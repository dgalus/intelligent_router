#include "common.h"

const char* programName;

void* xmalloc(size_t size)
{
    void *ptr = malloc(size);
    if(ptr == NULL)
    {
        abort();
    }
    else
    {
        return ptr;
    }
}

void* xrealloc(void *ptr, size_t size)
{
    ptr = realloc(ptr, size);
    if(ptr == NULL)
    {
        abort();
    }
    else
    {
        return ptr;
    }
}

char* xstrdup(const char* s)
{
    char* copy = strdup(s);
    if(copy == NULL)
    {
        abort();
    }
    else
    {
        return copy;
    }
}

void error(const char* cause, const char* message)
{
    fprintf(stderr, "%s: error: (%s) %s\n", programName, cause, message);
    exit(EXIT_FAILURE);
}


void systemError(const char* operation)
{
    error(operation, strerror(errno));
}

char* getSelfExecutableDirectory()
{
    int rval;
    char linkTarget[1024];
    char* lastSlash;
    size_t resultLength;
    char* result;

    rval = readlink("/proc/self/exe", linkTarget, sizeof(linkTarget));
    if(rval == -1)
    {
        abort();
    }
    else
    {
        linkTarget[rval] = '\0';
    }
    lastSlash = strrchr(linkTarget, '/');
    if(lastSlash == NULL || lastSlash == linkTarget)
    {
        abort();
    }
    resultLength = lastSlash - linkTarget;
    result = (char *) xmalloc(resultLength+1);
    strncpy(result, linkTarget, resultLength);
    result[resultLength] = '\0';

    return result;
}
