#ifndef HELPERS_H
#define HELPERS_H

#include <algorithm>
#include <assert.h>
#include <string>
#include <fstream>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <inttypes.h>
#include <algorithm>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/ether.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <dirent.h>

#include "errors.h"
#include "configuration.h"
#include "policyfactory.h"
#include "quagga_adapter.h"

#ifdef HAVE_ST_BIRTHTIME
#define birthtime(x) x.st_birthtime
#else
#define birthtime(x) x.st_ctime
#endif

class FileWriter
{
public:
    FileWriter(const std::string path);
    ~FileWriter();
    void writeToFile(const std::string content);
    void appendToFile(const std::string content);
private:
    int fileDescriptor;
    std::string path;
};

class FileReader
{
public:
    static std::string readFile(const std::string & path);
    static std::string readCHTMLFile(const std::string & path);
};

class FileHelper
{
public:
  static time_t getFileCreationTimestamp(std::string path);
};

class StringHelper
{
public:
    static bool hasEnding(std::string const & fullString, std::string const & ending);
    static std::vector<std::string> explode(const std::string & str, const char & ch);
    static void removeNewLineChars(std::string & s);
    static std::string replaceAll(std::string str, const std::string & from, const std::string & to);
    static void removeWhiteChars(std::string & input);
};


/* http://www.adp-gmbh.ch/cpp/common/base64.html */

class Base64
{
private:
  static const std::string base64Chars(void);
  static inline bool isBase64(unsigned char c);
public:
  static std::string decode(std::string const &);
};

class IP
{
private:
  static const std::vector<unsigned char> validMaskOctets();
public:
  static bool isValidHostIP(std::string & addr, std::string & msk);
  static std::string maskToString(const std::string & input);
  static std::vector<std::string> const getMasks();
  static bool isNetworkOrHostAddress(std::string & addr, std::string & msk);
  static bool isValidAddrMask(std::string & addrMsk);
};

class Port
{
public:
  static inline bool isValidPort(int port)
  {
    return (port > 0 && port < 65536);
  }
};

class Interface
{
public:
  static std::vector<std::string> getInterfaces();
  static bool isInterfaceUp(std::string interface);
  static std::string getHWAddr(const std::string & interface);
  static std::string getIPAddr(const std::string & interface);
  static std::string getMask(const std::string & interface);
  static void setInterfaceStatus(std::string & interfaceName, bool enabled);
  static void setInterfaceAddress(std::string interfaceName, std::string & ipAddress, std::string & subnetMask);
};

class Routing
{
public:
  static std::vector<std::string> getDNSServers();
  static void setNameservers(std::vector<std::string> & addresses);
  static std::string getDefaultGW();
  static void enableRoutingProtocol(const std::string & interfaceName, const std::string & protocol);
  static void disableRoutingProtocol(const std::string & interfaceName, const std::string & protocol);
  static std::string getStaticRoutes();
  static void addStaticRoute(const std::string & targetAddressMask, const std::string & viaAddress, const std::string & viaInterface);
  static void removeStaticRoute(const std::string & targetAddress, const std::string & netmask, const std::string & interface);
};

class Firewall
{
public:
  static bool getAdaptativeFirewallState();
  static std::string getNonAdaptativeFirewallLoadedPolicy();
  static std::string getRules();
  static std::vector<std::string> getAvailablePolicies();
  static void enableAdaptativeFirewall();
  static void disableAdaptativeFirewall();
  static void applyNonAdaptativeFirewallPolicy(std::string & policyName);
  static void forwardSinglePort(std::string app, uint16_t portOut, uint16_t portIn, bool isTCP, std::string ipAddress, bool enabled);
  static void flushAll();
  static void update();
};

class Service
{
public:
  static void start(std::string name);
  static void stop(std::string name);
  static bool isServiceRunning(std::string & name);
};

class Management
{
public:
  static int getWWWInterfacePort();
  static void disableWWWInterface();
  static void setWWWInterfacePort(uint16_t port);
  static void performSoftReset();
};

#endif // HELPERS_H
