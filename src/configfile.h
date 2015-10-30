#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#include <algorithm>
#include <exception>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "errors.h"
#include "helpers.h"
#include "log.h"

class ConfigFile
{
public:
  ConfigFile();
  ~ConfigFile();
  ConfigFile(const std::string & path);
  std::string getPath();
  void setPath(const std::string & path);
  template<typename T> T getConfig();
protected:
  void readConfigFile();
  int fileDescriptor;
  std::string path;
  std::string content;
};

class PoliciesConfiguration : public ConfigFile
{
public:
  const std::vector<std::string> & getConfig();
private:
  void mapElements();
  std::vector<std::string> policiesConfiguration;
};

class CriterionsConfiguration : public ConfigFile
{
public:
  const std::vector<std::string> & getConfig();
private:
  void mapElements();
  std::vector<std::string> criterionsConfiguration;
};

class IncompleteConfigException : public std::exception
{
public:
    virtual const char* what() const throw()
    {
        return "Incomplete configuration file. Check if all mandatory arguments are set.";
    }
};

class ConfigFileSyntaxErrorException : public std::exception
{
public:
    virtual const char* what() const throw()
    {
        return "Configuration file syntax error. Check criterions configuration file.";
    }
};

class CritetionsNotDefinedException : public std::exception
{
public:
    virtual const char* what() const throw()
    {
        return "No criterions defined. Check configuration file";
    }
};

class PoliciesNotDefinedException : public std::exception
{
public:
    virtual const char* what() const throw()
    {
        return "No policies defined. Check configuration file";
    }
};

#endif
