#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "adaptive_firewall_adapter.h"
#include "quagga_adapter.h"

static std::string usageTemplate = "Usage: show <module> \n";

void printUsage(int errorCode)
{
  fprintf(stderr, "%s\n", usageTemplate.c_str());
  exit(errorCode);
}

int main(int argc, char* argv[])
{
  if(argc < 2)
  {
    printUsage(1);
  }
  std::string module = std::string(argv[1]);
  if(module == "firewall")
  {
    AdaptiveFirewallAdapter::executeShowCommand(argc-2, argv+2);
  }
  else
  {
    QuaggaAdapter::executeCommand(argc, argv, false);
  }
  return 0;
}
