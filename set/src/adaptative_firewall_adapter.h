#ifndef ADAPTATIVE_FIREWALL_ADAPTER_H_
#define ADAPTATIVE_FIREWALL_ADAPTER_H_

#include <iostream>
#include <string>

class AdaptativeFirewallAdapter
{
public:
  static void executeCommand(int argc, char* argv[]);
private:
  static void disableAdaptativeFirewall();
  static void enableAdaptativeFirewall();
  static void disableManualFirewall();
  static void enableManualFirewall(std::string policyFilePath);
  static void printUsage();
  static constexpr char const * usageTemplate = "Usage: set firewall <status> <option>\nStatus:\n\tenable\n\tdisable\nAvailable options:\n\tmanual <path_to_policy_file>\n\tadaptative\n";
};

#endif
