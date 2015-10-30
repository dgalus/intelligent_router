#include "log.h"

Log::Log()
{
}

Log::~Log()
{
    delete this;
}

void Log::write(const std::string & content)
{
    if(fw != NULL)
    {
        time_t t = time(NULL);
        struct tm tms = *localtime(&t);
        fw->writeToFile(
                    "[" + std::to_string(tms.tm_year+1900)
                    + "-" + std::to_string(tms.tm_mon + 1)
                    + "-" + std::to_string(tms.tm_mday)
                    + " " + std::to_string(tms.tm_hour)
                    + "-" + std::to_string(tms.tm_min)
                    + "-" + std::to_string(tms.tm_sec)
                    + "] "+ content
                    );
    }
    else
    {
        std::cerr << "Path: " << path << " is unreachable or not set!" << std::endl;
        exit(UNABLE_TO_WRITE_TO_LOG_FILE);
    }
}

void Log::append(const std::string & content)
{
    if(fw != NULL)
    {
        time_t t = time(NULL);
        struct tm tms = *localtime(&t);
        fw->appendToFile(
                    "[" + std::to_string(tms.tm_year+1900)
                    + "-" + std::to_string(tms.tm_mon + 1)
                    + "-" + std::to_string(tms.tm_mday)
                    + " " + std::to_string(tms.tm_hour)
                    + "-" + std::to_string(tms.tm_min)
                    + "-" + std::to_string(tms.tm_sec)
                    + "] "+ content
                    );
    }
    else
    {
        std::cerr << "Path: " << path << " is unreachable or not set!" << std::endl;
        exit(UNABLE_TO_WRITE_TO_LOG_FILE);
    }
}

void Log::setPath(const std::string & path)
{
    this->path = path;
    this->fw = new FileWriter(path);
}
