#include "adaptative_firewall_adapter.h"

void AdaptativeFirewallAdapter::executeCommand(int argc, char* argv[])
{
  if(std::string(argv[0]) == "enable")
  {
    if(std::string(argv[1]) == "manual")
    {
      enableManualFirewall(std::string(argv[2]));
    }
    else if(std::string(argv[1]) == "adaptative")
    {
      enableAdaptativeFirewall();
    }
    else
    {
      printUsage();
    }
  }
  else if(std::string(argv[0]) == "disable")
  {
    if(std::string(argv[1]) == "manual")
    {
      disableManualFirewall();
    }
    else if(std::string(argv[1]) == "adaptative")
    {
      disableAdaptativeFirewall();
    }
    else
    {
      printUsage();
    }
  }
  else
  {
    printUsage();
  }
}

void AdaptativeFirewallAdapter::disableAdaptativeFirewall()
{

}

void AdaptativeFirewallAdapter::enableAdaptativeFirewall()
{

}

void AdaptativeFirewallAdapter::disableManualFirewall()
{

}

void AdaptativeFirewallAdapter::enableManualFirewall(std::string policyFilePath)
{

}

void AdaptativeFirewallAdapter::printUsage()
{
  printf("%s\n", usageTemplate);
}
