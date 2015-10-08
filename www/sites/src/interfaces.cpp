#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/ether.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <ifaddrs.h>

#define __SAVE
#define __MODULE

#include "../../src/helpers.h"
#include "../../src/server.h"

std::vector<std::string> interfaces;

static std::string masks[33] = {
	"0.0.0.0",
	"128.0.0.0",
	"192.0.0.0",
	"224.0.0.0",
	"240.0.0.0",
	"248.0.0.0",
	"252.0.0.0",
	"254.0.0.0",
	"255.0.0.0",
	"255.128.0.0",
	"255.192.0.0",
	"255.224.0.0",
	"255.240.0.0",
	"255.248.0.0",
	"255.252.0.0",
	"255.254.0.0",
	"255.255.0.0",
	"255.255.128.0",
	"255.255.192.0",
	"255.255.224.0",
	"255.255.240.0",
	"255.255.248.0",
	"255.255.252.0",
	"255.255.254.0",
	"255.255.255.0",
	"255.255.255.128",
	"255.255.255.192",
	"255.255.255.224",
	"255.255.255.240",
	"255.255.255.248",
	"255.255.255.252",
	"255.255.255.254",
	"255.255.255.255"
};

extern "C" std::string generateInterfacesTable()
{
	std::string out = "";
	out.append("<table class=\"table\"><tr>");
	std::for_each(interfaces.begin(), interfaces.end(), [&out](std::string & s){
		out.append("<td><input type=\"checkbox\" name=\"enable_" + s + "\" value=\"1\" /> " + s + "</td>");
	});
	out.append("</tr></table>");
	return out;
}

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

extern "C" std::string getHWAddr(const std::string & interface)
{
	char buf[19];
	memset(buf, 0, sizeof(buf));
	FILE *p;
	p = popen(std::string("cat /sys/class/net/").append(interface).append("/address").c_str(), "r");
	fgets(buf, 18, p);
	fclose(p);
	return std::string(buf);
}

extern "C" std::string getIPAddr(const std::string & interface)
{
	int fd;
	struct ifreq ifr;
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	ifr.ifr_addr.sa_family = AF_INET;
	strncpy(ifr.ifr_name, interface.c_str(), IFNAMSIZ-1);
	ioctl(fd, SIOCGIFADDR, &ifr);
	close(fd);
	return std::string(inet_ntoa(((struct sockaddr_in *) &ifr.ifr_addr)->sin_addr));
}

extern "C" bool isInterfaceUp(std::string interface)
{
	struct ifreq ifr;
	int sock = socket(PF_INET6, SOCK_DGRAM, IPPROTO_IP);
	memset(&ifr, 0, sizeof(ifr));
	strcpy(ifr.ifr_name, interface.c_str());
	if(ioctl(sock, SIOCGIFFLAGS, &ifr) < 0)
	{
		assert(false);
	}
	close(sock);
	return !!(ifr.ifr_flags & IFF_UP);
}

extern "C" std::string maskToString(const std::string & input)
{
	if(input.find("/") != std::string::npos)
	{
		std::string s = input.substr(input.find("/")+1, 2);
		uint8_t size = atoi(s.c_str());
		return masks[size];
	}
	else
	{
		return "";
	}
}

extern "C" std::string getMask(const std::string & interface)
{
	char buf[256];
	memset(buf, 0, sizeof(buf));
	FILE *p;
	p = popen(std::string("ip addr show " + interface + " | awk '/inet/ {print $2}'").c_str(), "r");
	fgets(buf, 256, p);
	fclose(p);
	return std::string(buf);
}

extern "C" std::string getDefaultGW()
{
	char buf[32];
	memset(buf, 0, sizeof(buf));
	FILE *p;
	p = popen(std::string("ip route show default | awk '/default/ {print $3}'").c_str(), "r");
	fgets(buf, 32, p);
	fclose(p);
	std::string ret(buf);
	StringHelper::removeNewLineChars(ret);
	return ret;
}

extern "C" std::string generateInterfaceSettings()
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
			bool enabled = isInterfaceUp(*it);
			out.append("<tr class=\"" + ((enabled) ? std::string("success") : std::string("error")) + "\"><td>Status: </td><td><b>" + ((enabled) ? std::string("Włączony") : std::string("Wyłączony")) + "</b></td></tr>");
			out.append("<tr><td>Adres IP:</td><td><input type=\"text\" name=\"lo_ip\" value=\"");
			out.append(getIPAddr(*it));
			close(fd);
			out.append("\" /></td></tr>");
			out.append("<tr><td>Maska podsieci:</td><td><input type=\"text\" name=\"" + *it + "_mask\" value=\"");
			out.append(maskToString(getMask(*it)));
			out.append("\" /></td>");
			out.append("</tr></table>");
		}
		else
		{
			out.append("<table class=\"table\"><tr>");
			bool enabled = isInterfaceUp(*it);
			out.append("<tr class=\"" + ((enabled) ? std::string("success") : std::string("error")) + "\"><td>Status: </td><td><b>" + ((enabled) ? std::string("Włączony") : std::string("Wyłączony")) + "</b></td></tr>");
			out.append("<tr><td>Adres MAC:</td><td><b>" + getHWAddr(*it) + "</b></td></tr>");
			out.append("<tr><td>Adres IP:</td><td><input type=\"text\" name=\"" + *it + "_ip\" value=\"");
			out.append(getIPAddr(*it));
			close(fd);
			out.append("\" /></td></tr>");
			out.append("<tr><td>Maska podsieci:</td><td><input type=\"text\" name=\"" + *it + "_mask\" value=\"");
			out.append(maskToString(getMask(*it)));
			out.append("\" /></td>");
			out.append("</tr></table>");
		}
	}
	return out;
}

extern "C" std::string generateDefaultGatewayTable()
{
	std::string out;
	out.append("<h4>Trasa domyślna</h4>");
	out.append("<table class=\"table\"><tr><td>Trasa domyślna</td><td><input type=\"text\" name=\"default_gw\" value=\"");
	out.append(getDefaultGW());
	out.append("\" /></td></tr></table>");
	return out;
}

extern "C" std::vector<std::string> getDNSServers()
{
	char buf[1024];
	std::vector<std::string> result;
	memset(buf, 0, sizeof(buf));
	FILE *p;
	p = popen(std::string("cat /etc/resolv.conf | grep nameserver | awk '/nameserver/ {print $2}'").c_str(), "r");
	while(fgets(buf, sizeof(buf)-1, p) != NULL)
	{
		result.push_back(std::string(buf));
		memset(buf, 0, sizeof(buf));
	}
	fclose(p);

	return result;
}

extern "C" std::string renderDNSTable()
{
	std::vector<std::string> results = getDNSServers();
	std::string out;
	out.append("<h4>Serwery DNS</h4>");
	out.append("<table class=\"table\">");
	for(unsigned int i = 0; i < results.size(); i++)
	{
		out.append("<tr><td><input type=\"text\" name=\"dns_" + std::to_string(i+1) + "\" value=\"");
		StringHelper::removeNewLineChars(results.at(i));
		out.append(results.at(i) + "\" /></td></tr>");
	}
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
	getInterfaces();
	FILE *fp;
	fp = fdopen(fileDescriptor, "w");
	assert(fp != NULL);
	fprintf(fp, FileReader::readCHTMLFile(std::string(HTML_TEMPLATES).append("interfaces.chtml")).c_str(),
		generateInterfacesTable().c_str(),
		generateInterfaceSettings().c_str(),
		generateDefaultGatewayTable().c_str(),
		renderDNSTable().c_str());
	fflush(fp);
}
