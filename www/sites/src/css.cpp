#include <assert.h>
#include <stdio.h>
#include <map>

#define __MODULE

#include "../../src/helpers.h"
#include "../../src/server.h"

extern "C" void moduleGenerate(int fileDescriptor, std::map<std::string, std::string> postVars)
{
	FILE *fp;
	fp = fdopen(fileDescriptor, "w");
	assert(fp != NULL);
	fprintf(fp, "%s", FileReader::readFile(std::string(CSS_TEMPLATES).append(postVars["addr"])).c_str());
	fflush(fp);
}
