#include <stdlib.h>
#include <assert.h>
#include <map>
#include <string>

#include "../../src/server.h"

extern "C" std::string renderWWWInterfaceTable()
{
	std::string ret = "";
	ret.append("<p><h4>Interfejs WWW</h4><table class=\"table\"><tr><td>Włączyć interfejs WWW</td>");
	ret.append("<td><input type=\"radio\" name=\"enable_www\" value=\"yes\" checked /> TAK <br><input type=\"radio\" name=\"enable_www\" value=\"no\" /> NIE</td>");
	ret.append("</tr><tr><td>Port interfejsu WWW:</td>");
	ret.append("<td><input type=\"text\" name=\"www_interface_port\" placeholder=\"" + std::to_string(Management::getWWWInterfacePort()) + "\" /></td>");
	ret.append("</tr></table></p>");
	return ret;
}

extern "C" std::string renderResetTable()
{
	std::string ret = "";
	ret.append("<p><h4>Wykonaj reset</h4><table class=\"table\"><tr>");
	ret.append("<td><input type=\"checkbox\" name=\"soft_reset\" value=\"soft_reset\" /> SOFT RESET</td>");
	ret.append("</tr></table></p>");
	return ret;
}

extern "C" void moduleGenerate(int fileDescriptor, std::map<std::string, std::string> postVars)
{
	FILE *fp;
	fp = fdopen(fileDescriptor, "w");
	assert(fp != NULL);
	fprintf(fp, FileReader::readFile(std::string(HTML_TEMPLATES).append("management.chtml")).c_str(),
		renderWWWInterfaceTable().c_str(),
		renderResetTable().c_str());
	fflush(fp);
}
