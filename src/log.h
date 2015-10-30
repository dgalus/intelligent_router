#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <string>
#include <time.h>

#include "helpers.h"
#include "singleton.h"

class Log : public Singleton<Log>
{
  friend class Singleton<Log>;
private:
  Log();
  ~Log();
  FileWriter* fw;
  std::string path;
public:
  void setPath(const std::string & path);
  void write(const std::string & content);
  void append(const std::string & content);
};

#endif
