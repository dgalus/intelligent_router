#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <map>
#include <string>

#include "../../src/server.h"

extern "C" void doAction(std::map<std::string, std::string> & postVars)
{
  std::map<std::string, std::string>::iterator it = postVars.find("enable_af");
  if(it != postVars.end() && it->second == "true")
  {
    Firewall::enableAdaptiveFirewall();
  }
  it = postVars.find("disable_af");
  if(it != postVars.end() && it->second == "true")
  {
    Firewall::disableAdaptiveFirewall();
  }
  it = postVars.find("policy");
  if(it != postVars.end())
  {
    if(it->second != "none")
    {
      Firewall::applyNonAdaptiveFirewallPolicy(postVars["policy"]);
    }
    else
    {
      Firewall::flushAll();
    }
  }
  std::string defaultMask = "0.0.0.0";
  for(int i = 1; i < 5; i++)
  {
    if(postVars.find("spf_app_" + std::to_string(i)) != postVars.end() && postVars.find("spf_portout_" + std::to_string(i)) != postVars.end() &&
        postVars.find("spf_portin_" + std::to_string(i)) != postVars.end() && postVars.find("spf_proto_" + std::to_string(i)) != postVars.end() &&
        postVars.find("spf_ip_" + std::to_string(i)) != postVars.end() && postVars.find("spf_en_" + std::to_string(i)) != postVars.end())
    {
      if(Port::isValidPort(std::stoul(postVars["spf_portout_" + std::to_string(i)])) && Port::isValidPort(std::stoul(postVars["spf_portin_" + std::to_string(i)])) &&
        postVars["spf_en_" + std::to_string(i)] == "true" && IP::isValidHostIP(postVars["spf_ip_" + std::to_string(i)], defaultMask) &&
        (postVars["spf_proto_" + std::to_string(i)] == "tcp" || postVars["spf_proto_" + std::to_string(i)] == "udp"))
      {
        Firewall::forwardSinglePort(postVars["spf_app_" + std::to_string(i)],
                                    std::stoul(postVars["spf_portout_" + std::to_string(i)]),
                                    std::stoul(postVars["spf_portin_" + std::to_string(i)]),
                                    postVars["spf_proto_" + std::to_string(i)] == "tcp",
                                    postVars["spf_ip_" + std::to_string(i)],
                                    true);
      }
    }
  }
}

extern "C" void moduleGenerate(int fileDescriptor, std::map<std::string, std::string> postVars)
{
  FILE *fp;
	fp = fdopen(fileDescriptor, "w");
	assert(fp != NULL);
  doAction(postVars);
  fprintf(fp, "<!DOCTYPE html><html><head><meta http-equiv=\"refresh\" content=\"0; url=/firewall\"></head></html>");
  fflush(fp);
}
