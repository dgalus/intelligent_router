#include "adaptive_firewall_adapter.h"

void AdaptiveFirewallAdapter::executeCommand(int argc, char* argv[])
{
  if(std::string(argv[0]) == "enable")
  {
    if(std::string(argv[1]) == "manual")
    {
      std::string policy(argv[2]);
      Firewall::applyNonAdaptiveFirewallPolicy(policy);
    }
    else if(std::string(argv[1]) == "adaptive")
    {
      Firewall::enableAdaptiveFirewall();
    }
    else
    {
      fprintf(stderr, "Invalid action. Valid actions are:\n\tmanual\n\tadaptive\n");
    }
  }
  else if(std::string(argv[0]) == "disable")
  {
    if(std::string(argv[1]) == "manual")
    {
      Firewall::flushAll();
    }
    else if(std::string(argv[1]) == "adaptive")
    {
      Firewall::disableAdaptiveFirewall();
    }
    else
    {
      fprintf(stderr, "Invalid action. Valid actions are:\n\tmanual\n\tadaptive\n");
    }
  }
  else
  {
    fprintf(stderr, "Invalid action. Valid actions are:\n\tenable\n\tdisable\n");
  }
}
