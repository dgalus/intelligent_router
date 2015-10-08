#ifndef HELPERS_H
#define HELPERS_H

#include <algorithm>
#include <string>

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#ifdef HAVE_ST_BIRTHTIME
#define birthtime(x) x.st_birthtime
#else
#define birthtime(x) x.st_ctime
#endif

class StringHelper
{
public:
  static void removeWhiteChars(std::string & input)
  {
    input.erase(
      std::remove_if(input.begin(), input.end(),
          [](char c) -> bool
          {
              return std::isspace<char>(c, std::locale::classic());
          }),
      input.end()
    );
  }
};


class FileHelper
{
public:
  static time_t getFileCreationTimestamp(std::string path)
  {
    struct stat st;
    memset(&st, 0, sizeof(struct stat));
    if(access(path.c_str(), F_OK) != -1)
    {
      stat(path.c_str(), &st);
    }
    return birthtime(st);
  }
};
#endif
