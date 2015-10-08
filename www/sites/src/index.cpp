#include <assert.h>
#include <stdio.h>
#include <map>
#include <vector>

#define __MODULE

#include "../../src/helpers.h"
#include "../../src/server.h"

typedef struct serv {
	std::string daemonName;
	std::string serviceName;
	serv(std::string _daemonName, std::string _serviceName) : daemonName(_daemonName), serviceName(_serviceName) {}
} Service;

static std::vector<Service> services = { 
	Service("quagga", "Quagga Router"),
	Service("iptables", "iptables firewall"),
	Service("adaptativefirewall", "Adaptative Firewall")
};

extern "C" bool checkIfServiceIsRunning(const std::string & name)
{
	char buf[256];
	memset(buf, 0, sizeof(buf));
	FILE *p;
	p = popen(std::string("service " + name + " status").c_str(), "r");
	fgets(buf, 256, p);
	fclose(p);
	if(std::string(buf).find("start/running") != std::string::npos || std::string(buf).find("is running") != std::string::npos)
	{
		return true;
	}
	else
	{
		return false;
	}
}

extern "C" std::string renderServicesTable()
{
	std::string out;
	out.append("<h4>Usługi</h4>");
	out.append("<table class=\"table\"><thead><tr><th>Usługa</th><th>Status</th><th>Akcje</th></tr></thead><tbody>");
	std::for_each(services.begin(), services.end(), [&out](Service & s) { 
		bool isRunning = checkIfServiceIsRunning(s.daemonName);
		out.append("<tr class=\"" + ((isRunning) ? std::string("success") : std::string("error")) + "\"><td>" + s.serviceName + "</td><td>" + ((isRunning) ? std::string("Działa") : std::string("Nie działa")) + "</td>");
		out.append("<td><button class=\"btn btn-link\">" + ((isRunning) ? std::string("Wyłącz") : std::string("Włącz")) + "</td></tr>");
	});
	out.append("</tbody></table>");
	return out;
}

extern "C" void moduleGenerate(int fileDescriptor, std::map<std::string, std::string> postVars)
{
	FILE *fp;
	fp = fdopen(fileDescriptor, "w");
	assert(fp != NULL);
	fprintf(fp, FileReader::readFile(std::string(HTML_TEMPLATES).append("index.chtml")).c_str(),
		renderServicesTable().c_str()
	);
	fflush(fp);
}
