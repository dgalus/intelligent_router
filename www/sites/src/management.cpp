#include <stdlib.h>
#include <assert.h>
#include <map>

#define __MODULE

#include "../../src/server.h"
#include "../../src/helpers.h"

extern "C" void moduleGenerate(int fileDescriptor, std::map<std::string, std::string> postVars)
{
	FILE *fp;
	fp = fdopen(fileDescriptor, "w");
	assert(fp != NULL);
	fprintf(fp, "%s", FileReader::readFile(std::string(HTML_TEMPLATES).append("management.chtml")).c_str());
	fflush(fp);
}
