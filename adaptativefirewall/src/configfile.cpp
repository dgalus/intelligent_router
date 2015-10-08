#include "configfile.h"

ConfigFile::ConfigFile()
{ }

ConfigFile::~ConfigFile()
{ }

ConfigFile::ConfigFile(const std::string & path)
{
  this->path = path;
  readConfigFile();
}

std::string ConfigFile::getPath()
{
    if(!(this->path.empty()))
    {
        return this->path;
    }
    else
    {
        return nullptr;
    }
}

void ConfigFile::setPath(const std::string &path)
{
    if(!path.empty())
    {
        this->path = path;
        readConfigFile();
    }
    else
    {
        Log::getInstance()->append("Given config file path is not valid! Quitting.");
        exit(CONFIG_NOT_FOUND);
    }
}

void ConfigFile::readConfigFile()
{
    const char* fileName = path.c_str();
    fileDescriptor = open(fileName, O_RDONLY);
    if(fileDescriptor > -1)
    {
        ssize_t ret;
        struct stat fs;
        fstat(fileDescriptor, &fs);
        int len = fs.st_size;
        if(len > 0)
        {
            char* buf;
            buf = (char *) malloc(len*sizeof(char));
            memset(buf, 0, len);

            char* begin;
            begin = buf;
            while(len != 0 && (ret = read(fileDescriptor, buf, len)) != 0)
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
            this->content = std::string(begin);
        }
    }
    close(fileDescriptor);
}

void MainConfiguration::mapElements()
{
    std::stringstream input(this->content);
    std::string line;
    std::vector<std::string> lines;

    // split by '\n'
    while(std::getline(input, line, '\n'))
    {
        lines.push_back(line);
    }

    // check syntax
    for(unsigned int i = 0; i < lines.size(); i++)
    {
        // check if line is commented out
        if(lines.at(i).substr(0, 1) != "#")
        {
            // do nothing with empty lines
            if(!lines.at(i).empty())
            {
                if(lines.at(i).find("=") == std::string::npos)
                {
                    std::cerr << "Syntax error in config file " << this->getPath() << ":" << std::to_string(i) << ". Quitting." << std::endl;
                    exit(CONFIG_SYNTAX_ERROR);
                }
            }
        }
    }

    // remove whitespaces
    std::for_each(lines.begin(), lines.end(), [](std::string & s){
      StringHelper::removeWhiteChars(s);
    });

    // map elements (split by '=')
    std::for_each(lines.begin(), lines.end(), [this](std::string & s){
        std::string token1 = s.substr(0, s.find("="));
        s.erase(0, s.find("=") + 1);
        this->map.insert(std::pair<std::string, std::string>(token1, s));
    });

    // check if valid arguments
    std::for_each(map.begin(), map.end(), [this](std::pair<std::string, std::string> p){
        if(std::find(validArguments.begin(), validArguments.end(), p.first) == validArguments.end())
        {
            std::cerr << "Invalid parameter in config file '" << p.first << "'. Quitting." << std::endl;
            exit(INVALID_PARAMETER_IN_CONFIG_FILE);
        }
    });

    mainConfig = (struct MainConfig *) malloc(sizeof(struct MainConfig));
    // parse map to MainConfig* structure
    mainConfig->adaptativeFirewallPidFile = map["adaptative_firewall_pid_file"].c_str();
    mainConfig->criterionDirectory = map["criterion_directory"].c_str();
    mainConfig->logPath = map["log_file"].c_str();
    mainConfig->policyDirectory = map["policy_directory"].c_str();
}

const MainConfig & MainConfiguration::getConfig()
{
    if(mainConfig == nullptr)
    {
        mapElements();
    }
    return *(this->mainConfig);
}

void PoliciesConfiguration::mapElements()
{
    std::stringstream input(this->content);
    std::string line;
    std::vector<std::string> lines;

    // split by '\n'
    while(std::getline(input, line, '\n'))
    {
        lines.push_back(line);
    }

    // remove commented lines
    for(std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); it++)
    {
        if(it->substr(0, 1) == "#")
        {
          continue;
        }
        this->policiesConfiguration.push_back(*it);
    }

    // remove whitespaces
    std::for_each(policiesConfiguration.begin(), policiesConfiguration.end(), [](std::string & s){
      StringHelper::removeWhiteChars(s);
    });

    // remove empty lines
    std::vector<std::string>::iterator it = std::remove_if(policiesConfiguration.begin(), policiesConfiguration.end(),
        [](const std::string & s) { return s.length() == 0; });
    policiesConfiguration.erase(it, policiesConfiguration.end());

    if(policiesConfiguration.size() <= 0)
    {
        exit(POLICIES_NOT_DEFINED);
    }
}

const std::vector<std::string> & PoliciesConfiguration::getConfig()
{
    if(policiesConfiguration.empty())
    {
        mapElements();
    }
    return this->policiesConfiguration;
}

void CriterionsConfiguration::mapElements()
{
  std::stringstream input(this->content);
  std::string line;
  std::vector<std::string> lines;

  while (std::getline(input, line, '\n'))
  {
    lines.push_back(line);
  }

  for(std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); it++)
  {
    if(it->substr(0, 1) == "#")
    {
      continue;
    }
    this->criterionsConfiguration.push_back(*it);
  }

  std::for_each(criterionsConfiguration.begin(), criterionsConfiguration.end(), [](std::string & s){
    StringHelper::removeWhiteChars(s);
  });

  std::vector<std::string>::iterator it = std::remove_if(criterionsConfiguration.begin(), criterionsConfiguration.end(), [](const std::string & s){
    return s.length() == 0;
  });
  criterionsConfiguration.erase(it, criterionsConfiguration.end());

  if(criterionsConfiguration.size() <= 0)
  {
    exit(POLICIES_NOT_DEFINED);
  }
}

const std::vector<std::string> & CriterionsConfiguration::getConfig()
{
  if(criterionsConfiguration.empty())
  {
    mapElements();
  }
  return this->criterionsConfiguration;
}
