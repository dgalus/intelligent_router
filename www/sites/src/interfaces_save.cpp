#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <map>
#include <string>

#define __SAVE
#define __INTERFACES_SAVE

#include "../../src/helpers.h"
#include "../../src/server.h"

std::vector<std::string> interfaces;

extern "C" void getInterfaces()
{
	system("ls /sys/class/net > /tmp/if");
	std::ifstream ifFile("/tmp/if");
	std::string line;
	while(std::getline(ifFile, line))
	{
		interfaces.push_back(line);
	}
	system("rm /tmp/if");
}

extern "C" void setInterfaceStatus(std::string & interfaceName, bool enabled)
{
	if(interfaceName == "lo")
	{
		return;
	}
	//cat /sys/class/net/wlp2s0/operstate
	FILE *fp;
	fp = popen(std::string("cat /sys/class/net/" + interfaceName + "/operstate").c_str(), "r");
	char buffer[1024];
	fgets(buffer, sizeof(buffer), fp);
	std::string state(buffer);
	if(state.find("up") != std::string::npos)
	{
		if(enabled == true)
		{
			return;
		}
		else
		{

		}
	}
	if(state.find("down") != std::string::npos)
	{
		if(enabled == false)
		{
			return;
		}
		else
		{
			
		}
	}
}

extern "C" void setDefaultGateway(std::string & gateway, std::string & interface)
{
	// route add default gw <IP> <interface>
	system("route add default gw " + gateway + " " + interface);
}

extern "C" void setNameservers(std::vector<std::string> & addresses)
{
	system("rm /etc/resolv.conf");
	std::for_each(addresses.begin(), addresses.end(),(std::string & s)[this]{
		system("echo \"nameserver " + s + "\" > /etc/resolv.conf");
	});
}

extern "C" void setInterfaceAddress(std::string interfaceName, std::string & ipAddress, std::string & subnetMask)
{

}

extern "C" void doAction(std::map<std::string, std::string> & postVars)
{
  if(!postVars["default_gw"].empty())
  {
    setDefaultGateway(postVars["default_gw"]);
  }
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
  setNameservers(nameservers);
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
      setInterfaceAddress(*it, address, mask);
    }
    if(postVars[("enable_" + *it)] == "1")
    {
      setInterfaceStatus(*it, true);
    }
    else
    {
      setInterfaceStatus(*it, false);
    }
  }
}

extern "C" void moduleGenerate(int fileDescriptor, std::map<std::string, std::string> postVars)
{
  getInterfaces();
  FILE *fp;
	fp = fdopen(fileDescriptor, "w");
	assert(fp != NULL);
  doAction(postVars);
  fprintf(fp, "<!DOCTYPE html><html><head><meta http-equiv=\"refresh\" content=\"0; url=/interfaces\"></head></html>");
  fflush(fp);
}
