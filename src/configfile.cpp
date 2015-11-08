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
            buf = (char *) malloc(len*sizeof(char)+1);
            memset(buf, 0, len*sizeof(char)+1);

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
    exit(CRITERIONS_NOT_DEFINED);
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
