#ifndef ADAPTATIVE_FIREWALL_H_
#define ADAPTATIVE_FIREWALL_H_

class AdaptativeFirewallAdapter
{
public:
  static void executeCommand(int argc, char* argv[]);
private:
  AdaptativeFirewallAdapter(){}
  ~AdaptativeFirewallAdapter(){}
};

#endif
