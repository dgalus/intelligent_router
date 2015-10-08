#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <string>

#include "errors.h"

class FileWriter
{
public:
    FileWriter(const std::string & path);
    ~FileWriter();
    void writeToFile(const std::string & content);
    void appendToFile(const std::string & content);
private:
    int fileDescriptor;
    std::string path;
};

#endif
