#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <string>
#include <time.h>

#include "errors.h"
#include "helpers.h"
#include "templates.h"

class Log
{
public:
    Log();
    ~Log();
    void setPath(const std::string path);
    void write(const std::string content);
    void append(const std::string content);
private:
    FileWriter* fw;
    std::string path;
};

typedef Singleton<Log> logg;

#endif // LOG_H
