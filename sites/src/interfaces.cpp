#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>

#include "../../src/server.h"

std::vector<std::string> interfaces;

extern "C" std::string renderInterfacesTable()
{
	std::string out = "";
	out.append("<table class=\"table\"><tr>");
	std::for_each(interfaces.begin(), interfaces.end(), [&out](std::string & s){
		out.append("<td><input type=\"checkbox\" name=\"enable_" + s + "\" value=\"1\" " + ((Interface::isInterfaceUp(s)) ? std::string("checked") : std::string("")) + " /> " + s + "</td>");
	});
	out.append("</tr></table>");
	return out;
}

extern "C" std::string renderInterfaceSettings()
{
	std::string out = "";
	int fd;
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	for(std::vector<std::string>::iterator it = interfaces.begin(); it != interfaces.end(); it++)
	{
		out.append("<h4>Ustawienia " + *it + "</h4>\n");
		if(it->find("lo") != std::string::npos) // if LO
		{
			out.append("<table class=\"table\"><tr>");
			bool enabled = Interface::isInterfaceUp(*it);
			out.append("<tr class=\"" + ((enabled) ? std::string("success") : std::string("error")) + "\"><td>Status: </td><td><b>" + ((enabled) ? std::string("Włączony") : std::string("Wyłączony")) + "</b></td></tr>");
			out.append("<tr><td>Adres IP:</td><td><input type=\"text\" name=\"lo_ip\" value=\"");
			out.append(Interface::getIPAddr(*it));
			close(fd);
			out.append("\" /></td></tr>");
			out.append("<tr><td>Maska podsieci:</td><td><input type=\"text\" name=\"" + *it + "_mask\" value=\"");
			out.append(IP::maskToString(Interface::getMask(*it)));
			out.append("\" /></td>");
			out.append("</tr></table>");
		}
		else
		{
			out.append("<table class=\"table\"><tr>");
			bool enabled = Interface::isInterfaceUp(*it);
			out.append("<tr class=\"" + ((enabled) ? std::string("success") : std::string("error")) + "\"><td>Status: </td><td><b>" + ((enabled) ? std::string("Włączony") : std::string("Wyłączony")) + "</b></td></tr>");
			out.append("<tr><td>Adres MAC:</td><td><b>" + Interface::getHWAddr(*it) + "</b></td></tr>");
			out.append("<tr><td>Adres IP:</td><td><input type=\"text\" name=\"" + *it + "_ip\" value=\"");
			out.append(Interface::getIPAddr(*it));
			close(fd);
			out.append("\" /></td></tr>");
			out.append("<tr><td>Maska podsieci:</td><td><input type=\"text\" name=\"" + *it + "_mask\" value=\"");
			out.append(IP::maskToString(Interface::getMask(*it)));
			out.append("\" /></td>");
			out.append("</tr></table>");
		}
	}
	return out;
}

extern "C" std::string renderDNSTable()
{
	std::vector<std::string> results = Routing::getDNSServers();
	std::string out;
	out.append("<h4>Serwery DNS</h4>");
	out.append("<table class=\"table\">");
	unsigned int i = 0;
	for( ; i < results.size(); i++)
	{
		out.append("<tr><td><input type=\"text\" name=\"dns_" + std::to_string(i+1) + "\" value=\"");
		StringHelper::removeNewLineChars(results.at(i));
		out.append(results.at(i) + "\" /></td></tr>");
	}
	out.append("<tr><td><input type=\"text\" name=\"dns_" + std::to_string(i) +"\" /></td></tr>");
	out.append("<tr><td><input type=\"text\" name=\"dns_" + std::to_string(++i) +"\" /></td></tr>");
	if(results.size() < 1)
	{
		out.append("<tr><td><input type=\"text\" name=\"dns_1\" /></td></tr>");
		out.append("<tr><td><input type=\"text\" name=\"dns_2\" /></td></tr>");
	}
	out.append("</table>");
	return out;
}

extern "C" void moduleGenerate(int fileDescriptor, std::map<std::string, std::string> postVars)
{
	interfaces = Interface::getInterfaces();
	FILE *fp;
	fp = fdopen(fileDescriptor, "w");
	assert(fp != NULL);
	fprintf(fp, FileReader::readCHTMLFile(std::string(HTML_TEMPLATES).append("interfaces.chtml")).c_str(),
		renderInterfacesTable().c_str(),
		renderInterfaceSettings().c_str(),
		renderDNSTable().c_str());
	fflush(fp);
}
