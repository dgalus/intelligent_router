#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/ip.h>
#include <net/if.h>
#include <linux/ethtool.h>
#include <linux/sockios.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <math.h>

#include <fstream>
#include <map>
#include <string>

#define SLEEP_TIME 2

struct Interface
{
  int index;
  int flags;
  long speed; /* IFF_UP */
  int duplex; /* DUPLEX_FULL, DUPLEX_HALF */
  char name[IF_NAMESIZE+1];
};

static int getInterfaceCommon(const int fd, struct ifreq * const ifr, struct Interface * const info)
{
  struct ethtool_cmd cmd;
  int result;
  if(ioctl(fd, SIOCGIFFLAGS, ifr) == -1)
  {
    info->flags = 0;
  }
  else
  {
    info->flags = ifr->ifr_flags;
  }
  ifr->ifr_data = (char *) &cmd;
  cmd.cmd = ETHTOOL_GSET; // Get settings
  if(ioctl(fd, SIOCETHTOOL, ifr) == -1)
  {
    info->speed = -1L;
    info->duplex = DUPLEX_UNKNOWN;
  }
  else
  {
    info->speed = ethtool_cmd_speed(&cmd);
    info->duplex = cmd.duplex;
  }
  do
  {
    result = close(fd);
  }
  while(result == -1 && errno == EINTR);
  if(result == -1)
  {
    return errno;
  }
  return 0;
}

static int getInterfaceByIndex(const int index, struct Interface * const info)
{
  int socketfd, result;
  struct ifreq ifr;
  if(index < 1 || !info)
  {
    return errno = EINVAL;
  }
  socketfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
  if(socketfd == -1)
  {
    return errno;
  }
  ifr.ifr_ifindex = index;
  if(ioctl(socketfd, SIOCGIFNAME, &ifr) == -1)
  {
    do
    {
      result = close(socketfd);
    }
    while(result == -1 && errno == EINTR);
    return errno = ENOENT;
  }
  info->index = index;
  strncpy(info->name, ifr.ifr_name, IF_NAMESIZE);
  info->name[IF_NAMESIZE] = '\0';
  return getInterfaceCommon(socketfd, &ifr, info);
}

std::string openFile(std::string path)
{
  std::ifstream infile(path.c_str());
  std::string output;
  std::string line;
  while(std::getline(infile, line))
  {
      output.append(line);
  }
  return output;
}

int getInterfaceUsageInPerc(struct Interface * const iface)
{
  unsigned long long RXbytes;
  unsigned long long TXbytes;
  unsigned long long RXbytesPrev;
  unsigned long long TXbytesPrev;
  unsigned long long RXDiff;
  unsigned long long TXDiff;
  unsigned long long speed;
  int ratio;

  std::string rxbytesfile = "/sys/class/net/" + std::string(iface->name) + "/statistics/rx_bytes";
  std::string txbytesfile = "/sys/class/net/" + std::string(iface->name) + "/statistics/tx_bytes";

  sscanf(openFile(rxbytesfile).c_str(), "%llu", &RXbytesPrev);
  sscanf(openFile(txbytesfile).c_str(), "%llu", &TXbytesPrev);

  sleep(SLEEP_TIME);

  sscanf(openFile(rxbytesfile).c_str(), "%llu", &RXbytes);
  sscanf(openFile(txbytesfile).c_str(), "%llu", &TXbytes);

  RXDiff = RXbytes - RXbytesPrev;
  TXDiff = TXbytes - TXbytesPrev;

  speed = iface->speed*1000000;

  if(iface->duplex == DUPLEX_FULL)
  {
    int rxratio = round((RXDiff/SLEEP_TIME)*100/speed);
    int txratio = round((TXDiff/SLEEP_TIME)*100/speed);
    if(rxratio > txratio)
    {
      ratio = rxratio;
    }
    else
    {
      ratio = txratio;
    }
  }
  else //if(iface->duplex == DUPLEX_HALF || iface->duplex == DUPLEX_UNKNOWN)
  {
    ratio = ((RXDiff + TXDiff)/SLEEP_TIME)*100/speed;
  }
  return ratio;
}

extern "C" std::map<int, std::string> getLevels()
{
  std::map<int, std::string> ret;
  return ret;
}

extern "C" int calculate()
{

  struct Interface iface;
  int arg;
  int max = -1;
  for(arg = 1; ; arg++)
  {
    if(getInterfaceByIndex(arg, &iface) != 0)
    {
      break;
    }
    if(std::string(iface.name) == "lo")
    {
      continue;
    }
    if(iface.flags & IFF_UP)
    {
      int ratio = getInterfaceUsageInPerc(&iface);
      if(ratio > max)
      {
        max = ratio;
      }
    }
    else
    {
      continue;
    }
  }

  return max;
}
