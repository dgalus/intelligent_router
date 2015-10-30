#include "quagga_adapter.h"

void QuaggaAdapter::executeCommand(int argc, char* argv[], bool enableMode)
{
  if(enableMode)
  {
    std::vector<std::string> vars(argv, argv+argc);
    std::string command = "configure terminal\" -c \"";
    std::for_each(vars.begin(), vars.end(), [&command](std::string & s){
      command.append(s).append(" ");
    });
    pipeToQuaggaShell(command);    
  }
  else
  {
    std::vector<std::string> vars(argv, argv+argc);
    if(vars.at(0).substr(0,2) == "./")
    {
      vars.at(0) = vars.at(0).substr(2, std::string::npos);
    }
    // concatenate vars
    std::string command = "";
    std::for_each(vars.begin(), vars.end(), [&command](std::string & s){
      command.append(s).append(" ");
    });
    pipeToQuaggaShell(command);
  }
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
