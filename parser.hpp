#include "unihead.h"
#include "filestruct.hpp"
#ifndef HOFF_PARSER
#define HOFF_PARSER
class HoffmanFileParser
{
private:
  char* Data;
  HoffmanFile* File;

  bool IsHoffmanFile(char* data)
  {
    const char magicw[] = {"hoff"};
    for (int i = 0; i < 4; i++)
    {
      if (magicw[i] != data[i])
      {
        return false;
      }
    }
    return true;
  }


  void ExtractLengths()
  {
     File->DataLength = 0;
     File->DictionaryLength = 0;
     File->FileNameLength = 0;
     File->PaddingSize = 0;

     for (int i = 4; i < 8; i++)
     {
       File->DataLength = File->DataLength >> 8;
       File->DataLength += Data[i] << 24;
     }
     for (int i = 8; i < 12; i++)
     {
       File->DictionaryLength = File->DictionaryLength >> 8;
       File->DictionaryLength += Data[i] << 24;
     }
     for (int i = 12; i < 16; i++)
     {
       File->FileNameLength = File->FileNameLength >> 8;
       File->FileNameLength += Data[i] << 24;
     }
     for (int i = 16; i < 20; i++)
     {
       File->PaddingSize = File->PaddingSize >> 8;
       File->PaddingSize += Data[i] << 24;
     }
  }

  void AllocateSizes()
  {
    File->FileName = new char[File->FileNameLength + 1];
    File->Dictionary = new char[File->DictionaryLength];
    File->Data = new char[File->DataLength];
    File->FileName[File->FileNameLength] = '\0';
  }

  void ExtractData()
  {
    for (int i = 20; i < File->FileNameLength + 20; i++)
    {
      File->FileName[i - 20] = Data[i];
    }
    for (int i = 20 + File->FileNameLength; i < File->FileNameLength + File->DictionaryLength + 20; i++)
    {
      File->Dictionary[i - File->FileNameLength - 20] = Data[i];
    }
    for (int i = 20 + File->FileNameLength + File->DictionaryLength; i < 20 + File->FileNameLength + File->DictionaryLength + File->DataLength; i++)
    {
      File->Data[i - 20 - File->FileNameLength - File->DictionaryLength] = Data[i];
    }

  }

public:
  HoffmanFileParser(char* data, unsigned int size)
  {
    File = new HoffmanFile;
    Data = data;
    if (size < 4 || !IsHoffmanFile(data))
      {
        cout << "Error: File isn't hoffman encoded\n";
        exit(1);
      }

    ExtractLengths();
    AllocateSizes();
    ExtractData();
  }

  HoffmanFile* GetFileHandle()
  {
    return File;
  }
};
#endif
