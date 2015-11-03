#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <map>
#include <string>

#include "../../src/server.h"

extern "C" void doAction(std::map<std::string, std::string> & postVars)
{
  if(postVars.find("enable_www") != postVars.end() && postVars["enable_www"] == "no" )
  {
    Management::disableWWWInterface();
  }
  Management::setWWWInterfacePort(std::stoi(postVars["www_interface_port"]));
  if(postVars.find("soft_reset") != postVars.end() && postVars["soft_reset"] == "soft_reset")
  {
    Management::performSoftReset();
  }
}

extern "C" void moduleGenerate(int fileDescriptor, std::map<std::string, std::string> postVars)
{
  FILE *fp;
	fp = fdopen(fileDescriptor, "w");
	assert(fp != NULL);
  doAction(postVars);
  fprintf(fp, "<!DOCTYPE html><html><head><meta http-equiv=\"refresh\" content=\"0; url=/management\"></head></html>");
  fflush(fp);
}
