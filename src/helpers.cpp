#include "helpers.h"

FileWriter::FileWriter(const std::string path)
{
  this->path = path;
}

FileWriter::~FileWriter()
{
  close(fileDescriptor);
  delete this;
}

void FileWriter::writeToFile(const std::string content)
{
  size_t bytesWritten;

  if(access(path.c_str(), F_OK) != -1)
  {
      remove(path.c_str());
  }

  fileDescriptor = open(path.c_str(), O_WRONLY | O_CREAT, 0666);
  if(fileDescriptor == -1)
  {
      exit(UNABLE_TO_WRITE_TO_LOG_FILE);
  }
  bytesWritten = write(fileDescriptor, content.c_str(), content.size());
  if(bytesWritten != content.size())
  {
      exit(ERROR_WHILE_WRITING_TO_LOG);
  }
  close(fileDescriptor);
}

void FileWriter::appendToFile(const std::string content)
{
  size_t bytesWritten;

  fileDescriptor = open(path.c_str(), O_WRONLY | O_APPEND, 0666);
  if(fileDescriptor == -1)
  {
      exit(UNABLE_TO_WRITE_TO_LOG_FILE);
  }

  bytesWritten = write(fileDescriptor, content.c_str(), content.size());
  if(bytesWritten != content.size())
  {
      exit(ERROR_WHILE_WRITING_TO_LOG);
  }

  close(fileDescriptor);
}

std::string FileReader::readFile(const std::string & path)
{
  std::ifstream infile(path.c_str());
  std::string output;
  std::string line;
  while(std::getline(infile, line))
  {
      output.append(line);
      output.append("\n");
  }
  return output;
}

std::string FileReader::readCHTMLFile(const std::string & path)
{
  std::ifstream infile(path.c_str());
  std::string output;
  std::string line;
  while(std::getline(infile, line))
  {
      output.append(line);
  }
  std::string ret = StringHelper::replaceAll(output, "\n", "\r\n");
  return ret;
}

time_t FileHelper::getFileCreationTimestamp(std::string path)
{
    struct stat st;
    memset(&st, 0, sizeof(struct stat));
    if(access(path.c_str(), F_OK) != -1)
    {
      stat(path.c_str(), &st);
    }
    return birthtime(st);
}

bool StringHelper::hasEnding(std::string const & fullString, std::string const & ending)
{
  if(fullString.length() >= ending.length())
  {
    return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
  }
  else
  {
    return false;
  }
}

std::vector<std::string> StringHelper::explode(const std::string & str, const char & ch)
{
  std::string next;
  std::vector<std::string> result;
  for(std::string::const_iterator it = str.begin(); it != str.end(); it++)
  {
    if(*it == ch)
    {
      if(!next.empty())
      {
        result.push_back(next);
        next.clear();
      }
    }
    else
    {
      next += *it;
    }
  }
  if(!next.empty())
  {
    result.push_back(next);
  }
  return result;
}

void StringHelper::removeNewLineChars(std::string & s)
{
  s.erase(std::remove_if(s.begin(), s.end(), [](char c){ return (c == '\n' || c == '\r'); }), s.end());
}

std::string StringHelper::replaceAll(std::string str, const std::string & from, const std::string & to)
{
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos)
    {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    return str;
}

void StringHelper::removeWhiteChars(std::string & input)
{
	input.erase(std::remove_if(input.begin(), input.end(), [](char c) -> bool {
		return std::isspace<char>(c, std::locale::classic());
	}), input.end());
}

const std::string Base64::base64Chars()
{
  return "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
}

bool Base64::isBase64(unsigned char c)
{
  return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string Base64::decode(std::string const & encodedString)
{
  int in_len = encodedString.size();
  int i = 0;
  int j = 0;
  int in_ = 0;
  unsigned char char_array_4[4], char_array_3[3];
  std::string ret;

  while (in_len-- && ( encodedString[in_] != '=') && isBase64(encodedString[in_]))
  {
      char_array_4[i++] = encodedString[in_]; in_++;
      if (i ==4)
      {
          for (i = 0; i <4; i++)
          {
              char_array_4[i] = base64Chars().find(char_array_4[i]);
          }
          char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
          char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
          char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
          for (i = 0; (i < 3); i++)
          {
              ret += char_array_3[i];
          }
          i = 0;
      }
  }
  if (i)
  {
      for (j = i; j <4; j++)
      {
          char_array_4[j] = 0;
      }
      for (j = 0; j <4; j++)
      {
          char_array_4[j] = base64Chars().find(char_array_4[j]);
      }
      char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
      char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
      char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
      for (j = 0; (j < i - 1); j++)
      {
          ret += char_array_3[j];
      }
  }
  return ret;
}

const std::vector<unsigned char> IP::validMaskOctets()
{
  std::vector<unsigned char> validMaskOctetsVector = { 255, 254, 252, 248, 240, 224, 192, 128, 0 };
  return validMaskOctetsVector;
}

bool IP::isValidAddrMask(std::string & addrMsk)
{
  char address[16];
  int maskSize;
  memset(&address, 0, sizeof(address));
  sscanf(addrMsk.c_str(), "%s/%d", address, &maskSize);
  if(maskSize > -1 && maskSize < 33)
  {
    std::string addr = std::string(address);
    std::string mask = getMasks()[maskSize];
    return isNetworkOrHostAddress(addr, mask);
  }
  else
  {
    return false;
  }
}

bool IP::isNetworkOrHostAddress(std::string & addr, std::string & msk)
{
  uint8_t a[4];
  uint8_t m[4];
  sscanf(addr.c_str(), "%" SCNd8 ".%" SCNd8 ".%" SCNd8 ".%" SCNd8 "", &a[0], &a[1], &a[2], &a[3]);
  sscanf(msk.c_str(), "%" SCNd8 ".%" SCNd8 ".%" SCNd8 ".%" SCNd8 "", &m[0], &m[1], &m[2], &m[3]);

  for(int i = 0; i < 4; i++)
  {
    if(std::find(validMaskOctets().begin(), validMaskOctets().end(), m[i]) == validMaskOctets().end())
    {
      return false;
    }
  }

  unsigned int address = a[0];
  address <<= 8;
  address += a[1];
  address <<= 8;
  address += a[2];
  address <<= 8;
  address += a[3];

  unsigned int mask = m[0];
  mask <<= 8;
  mask += m[1];
  mask <<= 8;
  mask += m[2];
  mask <<= 8;
  mask += m[3];

  if((address & mask) == address)
  {
    return true;
  }
  if((address | ~(mask)) == address)
  {
    return false;
  }
  if((address >= 0x7F000000 && address <= 0x7FFFFFFF) || (address <= 0x00FFFFFF))
  {
    return false;
  }
  return true;
}

bool IP::isValidHostIP(std::string & addr, std::string & msk)
{
  uint8_t a[4];
  uint8_t m[4];
  sscanf(addr.c_str(), "%" SCNd8 ".%" SCNd8 ".%" SCNd8 ".%" SCNd8 "", &a[0], &a[1], &a[2], &a[3]);
  sscanf(msk.c_str(), "%" SCNd8 ".%" SCNd8 ".%" SCNd8 ".%" SCNd8 "", &m[0], &m[1], &m[2], &m[3]);

  for(int i = 0; i < 4; i++)
  {
    if(std::find(validMaskOctets().begin(), validMaskOctets().end(), m[i]) == validMaskOctets().end())
    {
      return false;
    }
  }

  unsigned int address = a[0];
  address <<= 8;
  address += a[1];
  address <<= 8;
  address += a[2];
  address <<= 8;
  address += a[3];

  unsigned int mask = m[0];
  mask <<= 8;
  mask += m[1];
  mask <<= 8;
  mask += m[2];
  mask <<= 8;
  mask += m[3];

  if((address & mask) == address)
  {
    return false;
  }
  if((address | ~(mask)) == address)
  {
    return false;
  }
  if((address >= 0x7F000000 && address <= 0x7FFFFFFF) || (address <= 0x00FFFFFF))
  {
    return false;
  }
  return true;
}

std::vector<std::string> const IP::getMasks()
{
  std::vector<std::string> ret = {
    "0.0.0.0",
    "128.0.0.0",
    "192.0.0.0",
    "224.0.0.0",
    "240.0.0.0",
    "248.0.0.0",
    "252.0.0.0",
    "254.0.0.0",
    "255.0.0.0",
    "255.128.0.0",
    "255.192.0.0",
    "255.224.0.0",
    "255.240.0.0",
    "255.248.0.0",
    "255.252.0.0",
    "255.254.0.0",
    "255.255.0.0",
    "255.255.128.0",
    "255.255.192.0",
    "255.255.224.0",
    "255.255.240.0",
    "255.255.248.0",
    "255.255.252.0",
    "255.255.254.0",
    "255.255.255.0",
    "255.255.255.128",
    "255.255.255.192",
    "255.255.255.224",
    "255.255.255.240",
    "255.255.255.248",
    "255.255.255.252",
    "255.255.255.254",
    "255.255.255.255"
  };
  return ret;
}

std::string IP::maskToString(const std::string & input)
{
  if(input.find("/") != std::string::npos)
  {
    std::string s = input.substr(input.find("/")+1, 2);
    uint8_t size = atoi(s.c_str());
    if(size < 33)
    {
      return std::string(getMasks()[size]);
    }
    return "";
  }
  else
  {
    return "";
  }
}

std::vector<std::string> Interface::getInterfaces()
{
  std::vector<std::string> interfaces;
  system("ls /sys/class/net > /tmp/if");
  std::ifstream ifFile("/tmp/if");
  std::string line;
  while(std::getline(ifFile, line))
  {
    interfaces.push_back(line);
  }
  system("rm /tmp/if");
  return interfaces;
}

bool Interface::isInterfaceUp(std::string interface)
{
  struct ifreq ifr;
  int sock = socket(PF_INET6, SOCK_DGRAM, IPPROTO_IP);
  memset(&ifr, 0, sizeof(ifr));
  strcpy(ifr.ifr_name, interface.c_str());
  if(ioctl(sock, SIOCGIFFLAGS, &ifr) < 0)
  {
    assert(false);
  }
  close(sock);
  return !!(ifr.ifr_flags & IFF_UP);
}

std::string Interface::getHWAddr(const std::string & interface)
{
  char buf[19];
  memset(buf, 0, sizeof(buf));
  FILE *p;
  p = popen(std::string("cat /sys/class/net/").append(interface).append("/address").c_str(), "r");
  fgets(buf, 18, p);
  fclose(p);
  return std::string(buf);
}

std::string Interface::getIPAddr(const std::string & interface)
{
	int fd;
	struct ifreq ifr;
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	ifr.ifr_addr.sa_family = AF_INET;
	strncpy(ifr.ifr_name, interface.c_str(), IFNAMSIZ-1);
	ioctl(fd, SIOCGIFADDR, &ifr);
	close(fd);
	return std::string(inet_ntoa(((struct sockaddr_in *) &ifr.ifr_addr)->sin_addr));
}

std::string Interface::getMask(const std::string & interface)
{
	char buf[256];
	memset(buf, 0, sizeof(buf));
	FILE *p;
	p = popen(std::string("ip addr show " + interface + " | awk '/inet/ {print $2}'").c_str(), "r");
	fgets(buf, 256, p);
	fclose(p);
	return std::string(buf);
}

void Interface::setInterfaceStatus(std::string & interfaceName, bool enabled)
{
	if(interfaceName == "lo")
	{
		return;
	}
	FILE *fp;
	fp = popen(std::string("cat /sys/class/net/" + interfaceName + "/operstate").c_str(), "r");
	char buffer[1024];
	fgets(buffer, sizeof(buffer), fp);
	std::string state(buffer);
	if(state.find("up") != std::string::npos)
	{
		if(enabled == true)
		{
			return;
		}
		else
		{
			std::string command = "ip link set " + interfaceName + " down";
			system(command.c_str());
		}
	}
	if(state.find("down") != std::string::npos)
	{
		if(enabled == false)
		{
			return;
		}
		else
		{
			std::string command = "ip link set " + interfaceName + " up";
			system(command.c_str());
		}
	}
}

void Interface::setInterfaceAddress(std::string interfaceName, std::string & ipAddress, std::string & subnetMask)
{
	std::string command = "ip addr add " + ipAddress + "/" + subnetMask + " dev " + interfaceName;
	system(command.c_str());
}

bool Firewall::getAdaptiveFirewallState()
{
  std::string serviceName = "adaptivefirewall";
  bool isRunning = Service::isServiceRunning(serviceName);
  return isRunning;
}

std::string Firewall::getNonAdaptiveFirewallLoadedPolicy()
{
  return FileReader::readFile(LOADED_POLICY);
}

std::vector<std::string> Firewall::getAvailablePolicies()
{
  std::vector<std::string> v;
  struct dirent *entry;
  DIR *dir;
  dir = opendir(POLICY_DIRECTORY);
  while ((entry = readdir(dir)) != NULL)
  {
    std::string ent = std::string(entry->d_name);
    if(StringHelper::hasEnding(ent, ".so"))
    {
      v.push_back(StringHelper::replaceAll(ent, ".so", ""));
    }
  }
  return v;
}

std::string Firewall::getRules()
{
  std::string content = "";
  char buf[256];
  memset(buf, 0, sizeof(buf));
  FILE *p;
  p = popen("iptables -L -n", "r");
  if(p)
  {
    while(!feof(p))
    {
      if(fgets(buf, sizeof(256), p) != NULL)
      {
        content.append(std::string(buf));
        memset(buf, 0, sizeof(buf));
      }
    }
  }
  fclose(p);
  std::string validatedContent = StringHelper::replaceAll(content, "\n", "<br />");
  return validatedContent;
}

void Firewall::enableAdaptiveFirewall()
{
  system("nohup adaptivefirewall &");
}

void Firewall::flushAll()
{
  system("iptables -F");
  FileWriter* fw = new FileWriter(LOADED_POLICY);
  fw->writeToFile("");
}

void Firewall::disableAdaptiveFirewall()
{
  system("kill `pidof adaptivefirewall`");
}

void Firewall::applyNonAdaptiveFirewallPolicy(std::string & policyName)
{
  std::string serviceName = "adaptivefirewall";
  if(Service::isServiceRunning(serviceName))
  {
    disableAdaptiveFirewall();
  }
  std::string policyDirectory = std::string(POLICY_DIRECTORY);
  FileWriter* fw = new FileWriter(LOADED_POLICY);
  fw->writeToFile(policyName);
  std::string policyNameSo = policyName.append(".so");
  PolicyFactory::getInstance()->setPolicyDirectory(policyDirectory);
  Policy *p = PolicyFactory::getInstance()->open(policyNameSo.c_str());
  system(p->command);
  PolicyFactory::getInstance()->close(p);
}

void Firewall::forwardSinglePort(std::string app, uint16_t portOut, uint16_t portIn, bool isTCP, std::string ipAddress, bool enabled)
{
  // CHECK IF RULE EXISTS IN FILE, DELETE UNUSED RULES.
  std::string portOutStr = std::to_string(portOut);
  std::string portInStr = std::to_string(portIn);
  std::string proto = ((isTCP) ? std::string("tcp") : std::string("udp"));
  FileWriter* fw = new FileWriter(FIREWALL_RULES);
  fw->appendToFile("#" + app);
  fw->appendToFile((enabled ? std::string("") : std::string("#")) + "iptables -A FORWARD -m state -p " + proto + " -d " + ipAddress + " --dport " + portInStr + " --state NEW,ESTABLISHED,RELATED -j ACCEPT");
  fw->appendToFile((enabled ? std::string("") : std::string("#")) + "iptables -t nat -A PREROUTING -p " + proto + " --dport " + portOutStr + " -j DNAT --to-destination " + ipAddress + ":" + portInStr);
  update();
}

void Firewall::update()
{
  std::string command = "bash " + std::string(FIREWALL_RULES);
  system(command.c_str());
}

void Service::start(std::string name)
{
  if(name == "iptables")
  {
    system("modprobe ip_tables && modprobe iptable_filter");
    return;
  }
  if(name == "quagga" || name == "zebra")
  {
    system("/etc/init.d/quagga start");
    return;
  }
  if(name == "adaptivefirewall")
  {
    system("nohup adaptivefirewall &");
    return;
  }
  std::string command = name + " -d";
  system(command.c_str());
}

void Service::stop(std::string name)
{
  if(name == "iptables")
  {
    std::string command = "modprobe -r iptable_filter && modprobe -r ip_tables";
    system(command.c_str());
  }
  else
  {
    std::string command = "kill `pidof " + name + "`";
    system(command.c_str());
  }
}

bool Service::isServiceRunning(std::string & name)
{
  char buf[8];
  memset(buf, 0, sizeof(buf));
  FILE *p;
  if(name == "iptables")
  {
    p = popen(std::string("lsmod | grep ip_tables > /dev/null; echo $?").c_str(), "r");
    fgets(buf, 8, p);
    fclose(p);
    int val = atoi(buf);
    if(val == 0)
    {
      return true;
    }
    else
    {
      return false;
    }
  }
  p = popen(std::string("ps caux | grep " + name.substr(0, 15) + " > /dev/null; if [ $? -eq 0 ]; then echo \"yes\"; else echo \"no\"; fi").c_str(), "r");
  fgets(buf, 8, p);
  fclose(p);
  if(std::string(buf).find("yes") != std::string::npos)
  {
    return true;
  }
  else
  {
    return false;
  }
}

std::vector<std::string> Routing::getDNSServers()
{
	char buf[1024];
	std::vector<std::string> result;
	memset(buf, 0, sizeof(buf));
	FILE *p;
	p = popen(std::string("cat /etc/resolv.conf | grep nameserver | awk '/nameserver/ {print $2}'").c_str(), "r");
	while(fgets(buf, sizeof(buf)-1, p) != NULL)
	{
		result.push_back(std::string(buf));
		memset(buf, 0, sizeof(buf));
	}
	fclose(p);

	return result;
}

void Routing::setNameservers(std::vector<std::string> & addresses)
{
	system("rm /etc/resolv.conf");
	std::for_each(addresses.begin(), addresses.end(), [](std::string & s){
		std::string command = "echo \"nameserver " + s + "\" > /etc/resolv.conf";
		system(command.c_str());
	});
}

std::string Routing::getDefaultGW()
{
  char buf[32];
  memset(buf, 0, sizeof(buf));
  FILE *p;
  p = popen(std::string("ip route show default | awk '/default/ {print $3}'").c_str(), "r");
  fgets(buf, 32, p);
  fclose(p);
  std::string ret(buf);
  StringHelper::removeNewLineChars(ret);
  return ret;
}

void Routing::setDefaultGW(const std::string & address, const std::string & interface)
{
  std::string command = "route add default gw " + address + " " + interface;
  system(command.c_str());
}

void Routing::enableRoutingProtocol(const std::string & interfaceName, const std::string & protocol)
{
  std::string fileContent = FileReader::readFile(std::string(ROUTING_PROTOCOLS));
  std::vector<std::string> fileContentSplitted = StringHelper::explode(fileContent, '\n');
  bool found = false;
  std::for_each(fileContentSplitted.begin(), fileContentSplitted.end(), [&found, &interfaceName, &protocol](std::string & s) {
    if(s.find(interfaceName) != std::string::npos && s.find(protocol) != std::string::npos)
    {
      found = true;
    }
  });
  if(found == false)
  {
    std::string command = "configure terminal\" -c \"router " + protocol + "\" -c \"network " + interfaceName;
    QuaggaAdapter::pipeToQuaggaShell(command);
    fileContentSplitted.push_back(protocol + " " + interfaceName);
    FileWriter* fw = new FileWriter(ROUTING_PROTOCOLS);
    std::string textToStore = "";
    if(fileContentSplitted.begin() != fileContentSplitted.end())
    {
      textToStore.append(*(fileContentSplitted.begin()));
      textToStore.append("\n");
      for(std::vector<std::string>::iterator it = fileContentSplitted.begin() + 1; it != fileContentSplitted.end(); it++)
      {
        textToStore.append(*it);
        textToStore.append("\n");
      }
      fw->writeToFile(textToStore);
    }
  }
}

void Routing::disableRoutingProtocol(const std::string & interfaceName, const std::string & protocol)
{
  std::string fileContent = FileReader::readFile(ROUTING_PROTOCOLS);
  std::vector<std::string> fileContentSplitted = StringHelper::explode(fileContent, '\n');
  std::for_each(fileContentSplitted.begin(), fileContentSplitted.end(), [&interfaceName, &protocol](std::string & s){
    if(s.find(interfaceName) != std::string::npos && s.find(protocol) != std::string::npos)
    {
      std::string command = "configure terminal\" -c \"router " + protocol + "\" -c \"no network " + interfaceName;
      QuaggaAdapter::pipeToQuaggaShell(command);
    }
  });
  std::vector<std::string>::iterator elementsToRemove = std::remove_if(fileContentSplitted.begin(), fileContentSplitted.end(),[&protocol, &interfaceName](std::string & s){
    if(s.find(interfaceName) != std::string::npos && s.find(protocol) != std::string::npos)
    {
      return true;
    }
    else
    {
      return false;
    }
  });
  fileContentSplitted.erase(elementsToRemove, fileContentSplitted.end());
  FileWriter* fw = new FileWriter(ROUTING_PROTOCOLS);
  if(fileContentSplitted.begin() != fileContentSplitted.end())
  {
    fw->writeToFile(*(fileContentSplitted.begin()) + "\n");
    for(std::vector<std::string>::iterator it = fileContentSplitted.begin() + 1; it != fileContentSplitted.end(); it++)
    {
      fw->appendToFile(*it + "\n");
    }
  }
}

std::string Routing::getStaticRoutes()
{
  FILE *p;
  char buf[256];
  std::string output = "";
  memset(buf, 0, sizeof(buf));
  p = popen("ip route show", "r");
  while (fgets(buf, sizeof(buf), p) != NULL)
  {
    output.append(buf);
  }
  fclose(p);
  return StringHelper::replaceAll(output, "\n", "<br />");
}

void Routing::addStaticRoute(const std::string & targetAddressMask, const std::string & viaAddress, const std::string & viaInterface)
{
	std::string command = "ip route add " + targetAddressMask + " via " + viaAddress + " dev " + viaInterface;
  system(command.c_str());
}

void Routing::removeStaticRoute(const std::string & targetAddress, const std::string & netmask, const std::string & interface)
{
  std::vector<std::string> masks = IP::getMasks();
  int i;
  for(i = 0; i < 33; i++)
  {
    if(masks[i] == netmask)
    {
      break;
    }
  }
  if(i != 33)
  {
    std::string command = "ip route delete " + targetAddress + "/" + std::to_string(i) + " dev " + interface;
    system(command.c_str());
  }
}

int Management::getWWWInterfacePort()
{
	FILE *p;
	char buffer[8];
	p = fopen(WWW_PORT_FILE, "r");
	fgets(buffer, sizeof(buffer), p);
	fclose(p);
	return atoi(buffer);
}

void Management::disableWWWInterface()
{
  system("pkill `www`");
}

void Management::setWWWInterfacePort(uint16_t port)
{
  if(port == 80 || port > 1023)
  {
    FileWriter* fw = new FileWriter(WWW_PORT_FILE);
    fw->writeToFile(std::to_string(port));
  }
}

void Management::performSoftReset()
{
  system("reboot");
}
