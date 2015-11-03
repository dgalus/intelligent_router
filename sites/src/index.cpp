#include <assert.h>
#include <stdio.h>
#include <map>
#include <vector>

#include "../../src/server.h"

struct Serv {
	std::string daemonName;
	std::string serviceName;
	Serv(std::string _daemonName, std::string _serviceName) : daemonName(_daemonName), serviceName(_serviceName) {}
};

static std::vector<Serv> services = {
	Serv("zebra", "Zebra Router"),
	Serv("ripd", "RIP Daemon"),
	Serv("ripngd", "RIPng Daemon"),
	Serv("ospfd", "OSPF Daemon"),
	Serv("bgpd", "BGP Daemon"),
	Serv("ospf6d", "OSPF6 Daemon"),
	Serv("isisd", "IS-IS Daemon"),
	Serv("iptables", "iptables firewall"),
	Serv("adaptativefirewall", "Adaptative Firewall")
};

extern "C" std::string renderServicesTable()
{
	std::string out;
	out.append("<h4>Usługi</h4>");
	out.append("<table class=\"table\"><thead><tr><th>Usługa</th><th>Status</th><th>Akcje</th></tr></thead><tbody>");
	std::for_each(services.begin(), services.end(), [&out](Serv & s) {
		bool isRunning = Service::isServiceRunning(s.daemonName);
		out.append("<tr class=\"" + ((isRunning) ? std::string("success") : std::string("error")) + "\"><td>" + s.serviceName + "</td><td>" + ((isRunning) ? std::string("Działa") : std::string("Nie działa")) + "</td>");
		out.append("<td>" + ((isRunning) ? std::string("<input type=\"checkbox\" name=\"" + s.serviceName + "\" value=\"disable\" /> Wyłącz") : std::string("<input type=\"checkbox\" name=\"" + s.serviceName + "\" value=\"enable\" />Włącz")) + "</td></tr>");
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
