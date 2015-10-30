#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "adaptative_firewall_adapter.h"
#include "quagga_adapter.h"

static std::string usageTemplate = "";

void printUsage(int errorCode)
{
  fprintf(((errorCode == 0) ? stdout : stderr), "%s\n", usageTemplate.c_str());
  exit(errorCode);
}

int main(int argc, char* argv[])
{
  if(argc < 2)
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
    QuaggaAdapter::executeCommand(argc, argv, false);
  }
  return 0;
}
