#include "unihead.h"
#include "io.hpp"
#include "hoffman.hpp"
#include "composer.hpp"
#include "util.hpp"
#include "parser.hpp"

void CompressFile(char* filename, char* newfilename)
{
  char* data;
  size_t size;
  cout << "Reading File\n";
  data = ReadFile(filename, &size);

  HoffmanTree tree(data, size);
  HoffmanEncoder enc(&tree, data, size);
  cout << "Compressing Data\n";
  HoffmanFile* file = enc.CompressData(filename, strlen(filename));
  char* comp;
  cout << "Writing File\n";
  int compsize = HoffmanComposer::Compose(file, comp);
  WriteFile(newfilename, comp, compsize);
}

void DecompressFile(char* filename)
{
  char* data;
  size_t size;
  cout << "Reading file\n";
  data = ReadFile(filename, &size);

  cout << "Parsing file\n";
  HoffmanFileParser parser(data, size);
  HoffmanFile* file = parser.GetFileHandle();

  cout << "Parsing Dictionary\n";
  vector<HoffmanNode*> dic = Util::ParseDictionary(file->Dictionary, file->DictionaryLength);
  cout << "Building Tree\n";
  HoffmanTree* tree = new HoffmanTree(dic);

  cout << "Decoding Data\n";
  HoffmanDecoder decoder(tree, file);
  char* decdata;
  int decsize;

  cout << "Decompressing\n";
  decsize = decoder.DecompressData(decdata);

  cout << "Writing File\n";
  WriteFile(file->FileName, decdata, decsize);
}

int main(int argc, char** argv)
{
  if (argc != 4 && argc != 3)
  {
    cout << "Usage: " << argv[0] << " [d/c] <FileName> <NewFileName>\n";
    return 0;
  }

  if (argv[1][0] == 'c')
  {
    CompressFile(argv[2], argv[3]);
  }
  else if (argv[1][0] == 'd')
  {
    DecompressFile(argv[2]);
  }
  return 0;
}
