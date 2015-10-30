#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <map>
#include <string>

#include "../../src/server.h"

extern "C" void doAction(std::map<std::string, std::string> postVars)
{
  for(std::map<std::string, std::string>::iterator it = postVars.begin(); it != postVars.end(); it++)
  {
    if(it->second == "enable")
    {
      Service::start(it->first);
      continue;
    }
    if(it->second == "disable")
    {
      Service::stop(it->first);
    }
  }
}

extern "C" void moduleGenerate(int fileDescriptor, std::map<std::string, std::string> postVars)
{
  FILE *fp;
	fp = fdopen(fileDescriptor, "w");
	assert(fp != NULL);
  doAction(postVars);
  fprintf(fp, "<!DOCTYPE html><html><head><meta http-equiv=\"refresh\" content=\"0; url=/index\"></head></html>");
  fflush(fp);
}
