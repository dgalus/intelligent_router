#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <map>
#include <string>

#include "../../src/server.h"

typedef struct rp {
	std::string name;
	std::string displayName;
	rp(std::string _name, std::string _displayName) : name(_name), displayName(_displayName) { }
} RoutingProtocol;

std::vector<RoutingProtocol> routingProtocols = {
	RoutingProtocol("rip", "RIP"),
	RoutingProtocol("ripng", "RIPv2"),
	RoutingProtocol("ospf", "OSPF"),
	RoutingProtocol("ospf6", "OSPF6"),
	RoutingProtocol("isis", "IS-IS"),
	RoutingProtocol("bgp", "BGP")
};

std::vector<std::string> interfaces;

extern "C" void doAction(std::map<std::string, std::string> & postVars)
{
	std::map<std::string, bool> routingVars;
	for(std::vector<RoutingProtocol>::iterator it = routingProtocols.begin(); it != routingProtocols.end(); it++)
	{
		for(std::vector<std::string>::iterator it2 = interfaces.begin(); it2 != interfaces.end(); it2++)
		{
			std::map<std::string, std::string>::iterator finder = postVars.find((*it2) + "_" + (it->name));
			if(finder != postVars.end() && finder->second == "on")
			{
				Routing::enableRoutingProtocol(*it2, it->name);
			}
			else
			{
				Routing::disableRoutingProtocol(*it2, it->name);
			}
		}
	}
	std::string defaultMask = "0.0.0.0";
	for(int i=0; i < 5; i++)
	{
		std::map<std::string, std::string>::iterator routeFinder = postVars.find("route_" + std::to_string(i));
		if(routeFinder != postVars.end())
		{
			std::map<std::string, std::string>::iterator pathFinder = postVars.find("via_" + std::to_string(i));
			if(pathFinder != postVars.end())
			{
				std::string targetAddressMask = routeFinder->second;
				std::string viaAddress = pathFinder->second;
				if(IP::isValidAddrMask(targetAddressMask) && IP::isValidHostIP(viaAddress, defaultMask))
				{
					Routing::addStaticRoute(targetAddressMask, viaAddress, postVars["iface_" + std::to_string(i)]);
				}
			}
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
  fprintf(fp, "<!DOCTYPE html><html><head><meta http-equiv=\"refresh\" content=\"0; url=/routing\"></head></html>");
  fflush(fp);
}
