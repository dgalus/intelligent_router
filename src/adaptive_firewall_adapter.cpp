#include "adaptive_firewall_adapter.h"

void AdaptiveFirewallAdapter::executeSetCommand(int argc, char* argv[])
{
  if(std::string(argv[0]) == "adaptive")
  {
    Firewall::enableAdaptiveFirewall();
  }
  else if(std::string(argv[0]) == "non-adaptive")
  {
    Firewall::flushAll();
    Firewall::disableAdaptiveFirewall();
  }
  else if(std::string(argv[0]) == "policy" && argc == 2)
  {
    Firewall::flushAll();
    std::string policyName(argv[1]);
    Firewall::applyNonAdaptiveFirewallPolicy(policyName);
  }
  else
  {
    fprintf(stderr, "Invalid action. Valid actions are:\n\t<Quaggga EXEC Command>\n\tfirewall\n\t\tadaptive\n\t\tnon-adaptive\n\t\tpolicy <policy_name>\n");
  }
}

void AdaptiveFirewallAdapter::executeShowCommand(int argc, char* argv[])
{
  if(std::string(argv[0]) == "status")
  {
    std::string serviceName = "adaptivefirewall";
    if(Service::isServiceRunning(serviceName))
    {
      printf("Adaptive firewall is running.\n");
    }
    else
    {
      printf("Adaptive firewall is not running.\n");
    }
    printf("Loaded rules:\n%s\n", StringHelper::replaceAll(Firewall::getRules(), "<br />", "\n").c_str());
  }
  else if(std::string(argv[0]) == "policies" && argc == 1)
  {
    printf("Loaded rules:\n%s\n", StringHelper::replaceAll(Firewall::getRules(), "<br />", "\n").c_str());
  }
  else if(std::string(argv[0]) == "policies" && std::string(argv[1]) == "available")
  {
    std::vector<std::string> policies = Firewall::getAvailablePolicies();
    printf("Available policies:\n");
    for(std::vector<std::string>::iterator it = policies.begin(); it != policies.end(); it++)
    {
      printf("\t%s\n", it->c_str());
    }
  }
  else
  {
    fprintf(stderr, "Invalid action. Valid actions are:\n\t<Quaggga Command>\n\tfirewall\n\t\tstatus\n\t\tpolicies\n\t\tpolicies available\n");
  }
}
