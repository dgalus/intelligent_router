#ifndef QUAGGA_ADAPTER_H_
#define QUAGGA_ADAPTER_H_

#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

class QuaggaAdapter
{
public:
  static void executeCommand(int argc, char* argv[], bool enableMode);
private:
  static void pipeToQuaggaShell(std::string & command);
};

#endif
