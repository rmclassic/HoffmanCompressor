#ifndef HOFF_FILE_STRUCT
#define HOFF_FILE_STRUCT
struct HoffmanFile
{
  unsigned int DataLength;
  unsigned int DictionaryLength;
  unsigned int FileNameLength;
  unsigned int PaddingSize;
  char* FileName = nullptr;
  char* Dictionary = nullptr;
  char* Data = nullptr;
};
#endif
