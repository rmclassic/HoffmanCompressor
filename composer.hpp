#include "unihead.h"
#include "filestruct.hpp"
#ifndef HOFF_COMPOSER
#define HOFF_COMPOSER
class HoffmanComposer
{
public:
  static int Compose(HoffmanFile* file, char*& data)
  {
    unsigned int dictlen = file->DictionaryLength;
    unsigned int filenamesize = file->FileNameLength;
    unsigned int payloadsize = file->DataLength;
    int padding = file->PaddingSize;
    data = new char[20 + dictlen + filenamesize + payloadsize];
    const char magic[] = {"hoff"};
    for (int i = 0; i < 4; i++)
      data[i] = magic[i];

    for (int i = 4; i < 8; i++)
    {
      data[i] = payloadsize;
      payloadsize = payloadsize >> 8;
    }

    for (int i = 8; i < 12; i++)
    {
      data[i] = dictlen;
      dictlen = dictlen >> 8;
    }

    for (int i = 12; i < 16; i++)
    {
      data[i] = filenamesize;
      filenamesize = filenamesize >> 8;
    }

    for (int i = 16; i < 20; i++)
    {
      data[i] = padding;
      padding = padding >> 8;
    }
    dictlen = file->DictionaryLength;
    filenamesize = file->FileNameLength;
    payloadsize = file->DataLength;
    padding = file->PaddingSize;

    for (int i = 20; i < 20 + filenamesize; i++)
      data[i] = file->FileName[i - 20];

    for (int i = 20 + filenamesize; i < 20 + filenamesize + dictlen; i++)
      data[i] = file->Dictionary[i - 20 - filenamesize];

    for (int i = 20 + filenamesize + dictlen; i < 20 + filenamesize + dictlen + payloadsize; i++)
      data[i] = file->Data[i - 20 - filenamesize - dictlen];

    return 20 + dictlen + filenamesize + payloadsize;
  }
};
#endif
