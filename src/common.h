#ifndef COMMON_H
#define COMMON_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "server.h"


void* xmalloc(size_t size);
void* xrealloc(void *ptr, size_t size);
char* xstrdup(const char* s);
void error(const char* cause, const char* message);
void systemError(const char* operation);
char* getSelfExecutableDirectory();

#endif // COMMON_H
