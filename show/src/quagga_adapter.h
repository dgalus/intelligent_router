#ifndef QUAGGA_ADAPTER_H_
#define QUAGGA_ADAPTER_H_

class QuaggaAdapter
{
public:
  static void executeCommand(int argc, char* argv[]);
private:
  QuaggaAdapter(){}
  ~QuaggaAdapter(){}
};

#endif
