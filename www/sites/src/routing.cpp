#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <string>
#include <vector>

#define __MODULE

#include "../../src/helpers.h"
#include "../../src/server.h"

typedef struct rp {
	std::string name;
	std::string displayName;
	rp(std::string _name, std::string _displayName) : name(_name), displayName(_displayName) { }
} RoutingProtocol;

std::vector<std::string> interfaces;
std::vector<RoutingProtocol> routingProtocols = {
	RoutingProtocol("rip", "RIP"),
	RoutingProtocol("ripng", "RIPv2"),
	RoutingProtocol("ospf", "OSPF"),
	RoutingProtocol("bgp", "BGP")
};

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

extern "C" std::string renderRoutingProtocolsTable()
{
	std::string out;
	out.append("<h4>Protokoły routingu</h4>");
	out.append("<table class=\"table table-striped\"><thead><tr><th>Interfejs</th><th class=\"col-md-8\">Protokoły routingu</th></thead><tbody>");
	for(std::vector<std::string>::iterator it = interfaces.begin(); it != interfaces.end(); it++)
	{
		if(it->find("lo") == std::string::npos)
		{
			out.append("<tr><td>" + *it + "</td><td><div class=\"row\">");
			for(std::vector<RoutingProtocol>::iterator it2 = routingProtocols.begin(); it2 != routingProtocols.end(); it2++)
			{
				out.append("<div><input type=\"checkbox\" name=\"" + *it + "_" + it2->name + "\" /> " + it2->displayName + "&nbsp;</div>");
			}
			out.append("</div><td></tr>");
		}
	}
	out.append("</tbody></table>");
	return out;
}

extern "C" std::string renderStaticRoutingTable()
{
	std::string out;
	out.append("<h4>Routing statyczny</h4>");
	out.append("<table class=\"table table-striped\"><thead><tr><th>Trasa docelowa</th><th>Przez adres IP:</th><th>Przez interfejs:</th></tr></thead><tbody>");
	for(int i = 0; i < 5; i++)
	{
		out.append("<tr><td><input type=\"text\" name=\"route_" + std::to_string(i) + "\" /></td>");
		out.append("<td><input type=\"text\" name=\"via_" + std::to_string(i) + "\" /></td>");
		out.append("<td><select>");
		for(std::vector<std::string>::iterator it = interfaces.begin(); it != interfaces.end(); it++)
		{
			if(it->find("lo") == std::string::npos)
			{
				out.append("<option name=\"via_int_" + std::to_string(i) + "\" value=\"" + *it + "\">" + *it + "</option>");
			}
		}
		out.append("</select></td></tr>");
	}
	out.append("</tbody></table>");
	return out;
}

extern "C" void moduleGenerate(int fileDescriptor, std::map<std::string, std::string> postVars)
{
	getInterfaces();
	FILE *fp;
	fp = fdopen(fileDescriptor, "w");
	assert(fp != NULL);
	fprintf(fp, FileReader::readFile(std::string(HTML_TEMPLATES).append("routing.chtml")).c_str(),
		renderRoutingProtocolsTable().c_str(),
		renderStaticRoutingTable().c_str());
	fflush(fp);
}
