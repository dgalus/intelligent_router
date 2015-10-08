#ifndef HELPERS_H
#define HELPERS_H

#include <string>
#include <fstream>
#include <map>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <inttypes.h>
#include <algorithm>

#include "errors.h"

class FileWriter
{
public:
    FileWriter(const std::string path);
    ~FileWriter();
    void writeToFile(const std::string content);
    void append(const std::string content);
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

class StringHelper
{
public:
    static bool hasEnding(std::string const & fullString, std::string const & ending);
    static std::vector<std::string> explode(const std::string & str, const char & ch);
    static void removeNewLineChars(std::string & s);
    static std::string replaceAll(std::string str, const std::string & from, const std::string & to);
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
};

#endif // HELPERS_H
