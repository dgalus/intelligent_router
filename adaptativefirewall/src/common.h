#ifndef COMMON_H
#define COMMON_H

#include <string.h>

static void* xmalloc(size_t size)
{
  void* ptr = malloc(size);
  if(ptr == NULL)
  {
    abort();
  }
  else
  {
    return ptr;
  }
}

static char* xstrdup(const char* s)
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

#endif
