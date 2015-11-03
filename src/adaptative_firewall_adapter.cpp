#include "adaptative_firewall_adapter.h"

void AdaptativeFirewallAdapter::executeCommand(int argc, char* argv[])
{
  if(std::string(argv[0]) == "enable")
  {
    if(std::string(argv[1]) == "manual")
    {
      std::string policy(argv[2]);
      Firewall::applyNonAdaptativeFirewallPolicy(policy);
    }
    else if(std::string(argv[1]) == "adaptative")
    {
      Firewall::enableAdaptativeFirewall();
    }
    else
    {
      fprintf(stderr, "Invalid action. Valid actions are:\n\tmanual\n\tadaptative\n");
    }
  }
  else if(std::string(argv[0]) == "disable")
  {
    if(std::string(argv[1]) == "manual")
    {
      Firewall::flushAll();
    }
    else if(std::string(argv[1]) == "adaptative")
    {
      Firewall::disableAdaptativeFirewall();
    }
    else
    {
      fprintf(stderr, "Invalid action. Valid actions are:\n\tmanual\n\tadaptative\n");
    }
  }
  else
  {
    fprintf(stderr, "Invalid action. Valid actions are:\n\tenable\n\tdisable\n");
  }
}
