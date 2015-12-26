#include <assert.h>
#include <stdio.h>
#include <map>

#include "../../src/server.h"

extern "C" std::string renderAdaptiveFirewallTable()
{
	std::string ret = "";
	ret.append("<p><h4>Adaptacyjny firewall</h4><table class=\"table\">");
	bool firewallState = Firewall::getAdaptiveFirewallState();
	ret.append("<tr class=\"" + (firewallState ? std::string("success") : std::string("error")) + "\"><td>Status:</td><td><b>" + (firewallState ? std::string("Włączony") : std::string("Wyłączony")) + "</b></td><td>" + (firewallState ? std::string("<input type=\"checkbox\" name=\"disable_af\" value=\"true\" />Wyłącz") : std::string("<input type=\"checkbox\" name=\"enable_af\" value=\"true\" />Włącz")) + "</td></tr>");
	if(!firewallState)
	{
		ret.append("<tr class=\"warning\"><td>Zastosowana reguła dla nieadaptacyjnego firewalla:</td>");
		std::string policyName = Firewall::getNonAdaptiveFirewallLoadedPolicy();
		ret.append("<td><b>" + ((policyName == "") ? std::string("Nie wybrano") : policyName) + "</b></td>");
		ret.append("<td><select name=\"policy\" class=\"input-small\">");
		ret.append("<option value=\"none\">BRAK</option>");
		std::vector<std::string> availablePolicies = Firewall::getAvailablePolicies();
		std::for_each(availablePolicies.begin(), availablePolicies.end(), [&](std::string & s){
			ret.append("<option value=\"" + s + "\" " + ((s == policyName) ? std::string("selected") : std::string("")) + ">" + s + "</option>");
		});
		ret.append("</select></td>");
		ret.append("</tr>");
	}
	ret.append("</table><br /><br /></p>");
	return ret;
}

extern "C" std::string renderLoadedFirewallRulesPanel()
{
	std::string ret = "<h4>Zastosowane reguły firewalla</h4><p><code>" + Firewall::getRules() + "</code></p><br /><br />";
	return ret;
}

extern "C" std::string renderPortForwardingTable()
{
	std::string ret = "";
	ret.append("<p><h4>Przekierowanie pojedynczego portu</h4><table class=\"table\"><thead><tr><th>Aplikacja</th><th>Port zewnętrzny</th><th>Port wewnętrzny</th><th>Protokół</th><th>Adres IP</th><th>Włączone</th></tr></thead><tbody>");
	for(int i = 1; i < 5; i++)
	{
		ret.append("<tr><td><input type=\"text\" class=\"input-small\" name=\"spf_app_" + std::to_string(i) + "\" /></td>");
		ret.append("<td><input type=\"text\" class=\"input-small\" name=\"spf_portout_" + std::to_string(i) + "\" /></td>");
		ret.append("<td><input type=\"text\" class=\"input-small\" name=\"spf_portin_" + std::to_string(i) + "\" /></td>");
		ret.append("<td><select name=\"spf_proto_" + std::to_string(i) + "\" class=\"input-small\"><option selected=\"TCP\" value=\"TCP\">TCP</option><option value=\"UDP\">UDP</option></select></td>");
		ret.append("<td><input type=\"text\" class=\"input-small\" name=\"spf_ip_" + std::to_string(i) + "\" /></td><td><input type=\"checkbox\" value=\"true\" name=\"spf_en_" + std::to_string(i) + "\" /></td>");
		ret.append("</tr>");
	}
	ret.append("</tbody></table><br><br></p>");
	return ret;
}

extern "C" void moduleGenerate(int fileDescriptor, std::map<std::string, std::string> postVars)
{
	FILE *fp;
	fp = fdopen(fileDescriptor, "w");
	assert(fp != NULL);
	fprintf(fp, FileReader::readFile(std::string(HTML_TEMPLATES).append("firewall.chtml")).c_str(),
		renderAdaptiveFirewallTable().c_str(),
		renderLoadedFirewallRulesPanel().c_str(),
		renderPortForwardingTable().c_str());
	fflush(fp);
}
