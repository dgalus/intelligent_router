#ifndef ADAPTIVE_FIREWALL_ADAPTER_H_
#define ADAPTIVE_FIREWALL_ADAPTER_H_

#include <iostream>
#include <string>

#include "helpers.h"

class AdaptiveFirewallAdapter
{
public:
  static void executeSetCommand(int argc, char* argv[]);
  static void executeShowCommand(int argc, char* argv[]);
};

#endif
