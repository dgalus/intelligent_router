#include "quagga_adapter.h"

void QuaggaAdapter::executeCommand(int argc, char* argv[])
{
  std::vector<std::string> vars(argv, argv+argc);
  // concatenate vars
  std::string command = "";
  std::for_each(vars.begin(), vars.end(), [&command](std::string & s){
    command.append(s).append(" ");
  });
  pipeToQuaggaShell(command);
}

void QuaggaAdapter::pipeToQuaggaShell(std::string & command)
{
  std::string quaggaCommand = "vtysh -c \"" + command + "\"";
  FILE *in;
  char buff[512];
  if(!(in = popen(quaggaCommand.c_str(), "r")))
  {
    exit(1);
  }
  while(fgets(buff, sizeof(buff), in) != NULL)
  {
    std::cout << buff;
  }
  pclose(in);
}
