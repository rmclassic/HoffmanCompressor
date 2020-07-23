#include "unihead.h"
#ifndef IO
#define IO
int GetFileSize(const char* filename)
{
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    return in.tellg();
}

char* ReadFile(const char* filename, size_t* size)
{

  int filesize = GetFileSize(filename);
  *size = filesize;

  ifstream file(filename);
  if (!file)
  {
    cout << "File not found\n";
    exit(-1);
  }
  char* data = new char[filesize];

  file.read(data, filesize);
  return data;
}

void WriteFile(const char* filename, char* data, int size)
{
  ofstream file(filename);
  file.write(data, size);
}
#endif
