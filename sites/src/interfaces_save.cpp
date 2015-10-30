#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <map>
#include <string>

#include "../../src/server.h"

std::vector<std::string> interfaces;

extern "C" void doAction(std::map<std::string, std::string> & postVars)
{
  std::vector<std::string> nameservers;
  for(std::map<std::string, std::string>::iterator it = postVars.begin(); it != postVars.end(); it++)
  {
    if(it->first.find("dns") != std::string::npos)
    {
      std::string defaultGW = "0.0.0.0";
      if(IP::isValidHostIP(it->second, defaultGW))
      {
        nameservers.push_back(it->second);
      }
    }
  }
  Routing::setNameservers(nameservers);
  for(std::vector<std::string>::iterator it = interfaces.begin(); it != interfaces.end(); it++)
  {
    std::string address = "";
    std::string mask = "";
    bool addressSet = false;
    bool maskSet = false;
    for(std::map<std::string, std::string>::iterator it2 = postVars.begin(); it2 != postVars.end(); it2++)
    {
      if(it2->first.find(*it) != std::string::npos)
      {
        if(it2->first == (*it + "_ip"))
        {
          address = it2->second;
          addressSet = true;
        }
        else if(it2->first == (*it + "_mask"))
        {
          mask = it2->second;
          maskSet = true;
        }
        else
        { }
      }
    }
    if(addressSet && maskSet)
    {
      Interface::setInterfaceAddress(*it, address, mask);
    }
    if(postVars[("enable_" + *it)] == "1")
    {
      Interface::setInterfaceStatus(*it, true);
    }
    else
    {
      Interface::setInterfaceStatus(*it, false);
    }
  }
}

extern "C" void moduleGenerate(int fileDescriptor, std::map<std::string, std::string> postVars)
{
  interfaces = Interface::getInterfaces();
  FILE *fp;
	fp = fdopen(fileDescriptor, "w");
	assert(fp != NULL);
  doAction(postVars);
  fprintf(fp, "<!DOCTYPE html><html><head><meta http-equiv=\"refresh\" content=\"0; url=/interfaces\"></head></html>");
  fflush(fp);
}
