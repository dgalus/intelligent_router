#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <netinet/ip.h>
#include <net/if.h>
#include <unistd.h>
#include <fcntl.h>

#include <algorithm>
#include <map>
#include <fstream>
#include <string>
#include <vector>

#define SLEEP_TIME 2

struct InterfaceInfo
{
  std::string interfaceName;
  bool enabled;
  bool fullDuplex;
  unsigned long long RXUsage;
  unsigned long long TXUsage;
  unsigned long long RXSpeed;
  unsigned long long TXSpeed;
};

struct InterfaceHardwareInfo
{
  bool fullDuplex;
  unsigned long long RXSpeed;
  unsigned long long TXSpeed;
};

struct InterfaceLoad
{
  unsigned long long RXUsage;
  unsigned long long TXUsage;
};

class InterfaceStats
{
public:
  InterfaceStats()
  {
    collectInterfaces();
    std::for_each(interfaces.begin(), interfaces.end(), [this](std::string & interfaceName){
      InterfaceInfo ii;
      ii.interfaceName = interfaceName;
      ii.enabled = isInterfaceUp(interfaceName);
      InterfaceLoad il;
      il = getInterfaceLoad(interfaceName);
      ii.RXUsage = il.RXUsage;
      ii.TXUsage = il.TXUsage;
      InterfaceHardwareInfo ihwinfo;
      ii.RXSpeed = ihwinfo.RXSpeed;
      ii.TXSpeed = ihwinfo.TXSpeed;
      ii.fullDuplex = ihwinfo.fullDuplex;
      interfacesInfo.push_back(ii);
    });
  }
  std::vector<InterfaceInfo> & getInterfacesInfo()
  {
    return interfacesInfo;
  }

private:
  InterfaceLoad getInterfaceLoad(std::string & name)
  {
    InterfaceLoad il;
    std::string bytesReceived;
    std::string bytesSent;
    std::string bytesSentPrevious;
    std::string bytesReceivedPrevious;

    bytesReceivedPrevious = readFile("/sys/class/net/" + name + "/statistics/rx_bytes");
    bytesSentPrevious = readFile("/sys/class/net/" + name + "/statistics/tx_bytes");

    sleep(SLEEP_TIME);

    bytesReceived = readFile("/sys/class/net/" + name + "/statistics/rx_bytes");
    bytesSent = readFile("/sys/class/net/" + name + "/statistics/tx_bytes");

    unsigned long long rxusage = (std::strtoull(bytesReceived.c_str(), nullptr, 0)  - std::strtoull(bytesReceivedPrevious.c_str(), nullptr, 0))/SLEEP_TIME;
    unsigned long long txusage = (std::strtoull(bytesSent.c_str(), nullptr, 0)  - std::strtoull(bytesSentPrevious.c_str(), nullptr, 0))/SLEEP_TIME;

    il.RXUsage = rxusage;
    il.TXUsage = txusage;

    return il;
  }

  InterfaceHardwareInfo getInterfaceHardwareInfo(std::string & name)
  {
    InterfaceHardwareInfo ihwinfo;
    return ihwinfo;
  }

  inline void collectInterfaces()
  {
    system("ls /sys/class/net > /tmp/if");
    std::ifstream ifFile("/tmp/if");
    std::string line;
    while(std::getline(ifFile, line))
    {
      interfaces.push_back(line);
    }
    system("rm /tmp/if");
  }

  bool isInterfaceUp(std::string & interface)
  {
    struct ifreq ifr;
    int sock = socket(PF_INET6, SOCK_DGRAM, IPPROTO_IP);
    memset(&ifr, 0, sizeof(ifr));
    strcpy(ifr.ifr_name, interface.c_str());
    if(ioctl(sock, SIOCGIFFLAGS, &ifr) < 0)
    {
      perror("SIOCGIFFLAGES");
    }
    close(sock);
    return !!(ifr.ifr_flags & IFF_UP);
  }

  std::string readFile(std::string path)
  {
	  int fd;
    const char* filename = path.c_str();
    fd = open(filename, O_RDONLY);
    const char* begin = nullptr;
    if(fd > -1)
    {
      ssize_t ret;
      struct stat fs;
      fstat(fd, &fs);
      int len = fs.st_size;
      if(len > 0)
      {
        char* buf;
        buf = (char *) malloc(len*sizeof(char));
        memset(buf, 0, len);
        begin = buf;
        while(len != 0 && (ret = read(fd, buf, len)) !=0)
        {
          if(ret == -1)
          {
            if(errno == EINTR)
            {
              continue;
            }
            perror("read");
            break;
          }
          len -= ret;
          buf = buf + ret;
        }
      }
    }
    close(fd);
    if(begin != nullptr)
    {
      return std::string(begin);
    }
    else
    {
      return nullptr;
    }
  }

  std::vector<std::string> interfaces;
  std::vector<InterfaceInfo> interfacesInfo;
};

extern "C" std::map<int, std::string> getLevels()
{
  std::map<int, std::string> ret;
  return ret;
}

extern "C" int calculate()
{
  int val = 0;
  InterfaceStats *is = new InterfaceStats();
  std::vector<InterfaceInfo> interfacesInfo;
  interfacesInfo = is->getInterfacesInfo();
  delete is;

  // TODO:

  return val;
}
