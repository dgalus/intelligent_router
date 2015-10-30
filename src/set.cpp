#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "quagga_adapter.h"
#include "adaptative_firewall_adapter.h"

static std::string usageTemplate = "";

void printUsage(int errorCode)
{
  fprintf(((errorCode == 0) ? stdout : stderr), "%s\n", usageTemplate.c_str());
  exit(errorCode);
}

int main(int argc, char* argv[])
{
  if(argc < 3)
  {
    printUsage(0);
  }
  std::string module = std::string(argv[1]);
  if(module == "firewall")
  {
    AdaptativeFirewallAdapter::executeCommand(argc-2, argv+2);
  }
  else
  {
    QuaggaAdapter::executeCommand(argc-1, argv+1, true);
  }
  return 0;
}
