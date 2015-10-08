#include "filewriter.h"

FileWriter::FileWriter(const std::string & path)
{
  this->path = path;
}

FileWriter::~FileWriter()
{
  close(fileDescriptor);
  delete this;
}

void FileWriter::writeToFile(const std::string & content)
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

void FileWriter::appendToFile(const std::string & content)
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
