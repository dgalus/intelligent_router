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

  fileDescriptor = open(path.c_str(), O_WRONLY | O_CREAT, 0644);
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

void FileWriter::append(const std::string content)
{
  size_t bytesWritten;

  fileDescriptor = open(path.c_str(), O_APPEND);
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
