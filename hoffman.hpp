#include "unihead.h"
#include "tree.hpp"
#include "filestruct.hpp"
#ifndef HOFF_ENCODER
#define HOFF_ENCODER
class HoffmanEncoder
{
private:
  char* Data;
  size_t Size;
  HoffmanTree* Tree;
  size_t EncodedDataLength;

public:
  HoffmanEncoder(HoffmanTree* tree, char* data, size_t size)
  {
    Tree = tree;
    Data = data;
    Size = size;
  }

  HoffmanByte* EncodeData()
  {
    vector<HoffmanByte> encoded_data;
    for (int i = 0; i < Size; i++)
    {
      Tree->Encode(Data[i], &encoded_data, false);
    }
    EncodedDataLength = encoded_data.size();

    HoffmanByte* enc = new HoffmanByte[EncodedDataLength];

    for (int i = 0; i < EncodedDataLength; i++)
    {
      enc[i] = encoded_data[i];
    }
    return enc;
  }

  size_t CompressedLength(HoffmanByte* data, bool padding)
  {
    size_t size = 0;
    for (int i = 0; i < EncodedDataLength; i++)
      size += data[i].Size;
      if (!padding)
        return (size % 8 == 0) ? size / 8 : (size / 8) + 1;
      else return 8 - (size % 8);

  }

  void FillFileDictionary(HoffmanFile* file)
  {
    vector<HoffmanNode*> nodes = Tree->GetDictionaryAsVector();
    int nodesize = nodes.size();
    file->DictionaryLength = nodesize * 5;
    file->Dictionary = new char[nodesize * 5];
    int tempfreq;
    for (int i = 0, j = 0; i < nodesize * 5; i += 5, j++)
    {
      file->Dictionary[i] = nodes[j]->Content[0];
      tempfreq = nodes[j]->Frequency;
      for (int k = 1; k < 5; k++)
      {
        file->Dictionary[i + k] = tempfreq;
        tempfreq = tempfreq >> 8;
      }
    }
  }

  HoffmanFile* CompressData(char* filename, unsigned int filenamesize)
  {
    HoffmanFile* file = new HoffmanFile;
    FillFileDictionary(file);
    Tree->BuildTree();

    HoffmanByte* encoded = EncodeData();
    int compressedlen = CompressedLength(encoded, false);
    file->PaddingSize = CompressedLength(encoded, true);
    file->DataLength = compressedlen;
    file->FileName = filename;
    file->FileNameLength = filenamesize;
    file->Data = new char[compressedlen];
    for (int i = 0; i < compressedlen; i++)
      file->Data[i] = 0;
    int to_compress_index = 0;
    int compressed_index = 0;
    int filled = 0;
    int carry = 0;
    while (to_compress_index < EncodedDataLength)
    {

      carry = (8 - encoded[to_compress_index].Size - filled);
      if (carry > 0)
      {
        file->Data[compressed_index] += ((unsigned char)encoded[to_compress_index].Data) << carry;
        filled += encoded[to_compress_index].Size;
        to_compress_index++;
      }
      else if (carry == 0)
      {
        filled = 0;
        file->Data[compressed_index] += encoded[to_compress_index].Data;
        compressed_index++;
        to_compress_index++;
      }
      else if (carry < 0)
      {
        file->Data[compressed_index] += ((unsigned char)encoded[to_compress_index].Data) >> -carry;

        compressed_index++;
        file->Data[compressed_index] += ((unsigned char)encoded[to_compress_index].Data) << (8 + carry);
        to_compress_index++;
        filled = -carry;

      }
    }
    return file;
  }
};


class HoffmanDecoder
{
private:
  HoffmanFile* File;
  HoffmanTree* Tree;

public:
  HoffmanDecoder(HoffmanTree* tree, HoffmanFile* file)
  {
    Tree = tree;
    File = file;

    Tree->BuildTree();
  }

  int DecompressData(char*& data)
  {
    vector<char> bytes;
    char byte;
    char bitmask = 1 << 7;
    char tempbyte = File->Data[0];
    int untraversed_bits = 8;
    HoffmanNode* tempnode;

    for (int i = 0; i < File->DataLength;)
    {
      tempnode = Tree->Root();
      while (!tempnode->IsLeaf())
      {
        if (untraversed_bits == 0)
        {
          untraversed_bits = 8;
          i++;
          tempbyte = File->Data[i];
        }

        tempnode = tempnode->GetNode(tempbyte & bitmask);
        tempbyte = ((unsigned char)tempbyte) << 1;
        untraversed_bits--;
      }


      byte = tempnode->Content[0];

      bytes.push_back(byte);

      if (i == File->DataLength - 1 && untraversed_bits == File->PaddingSize)
        break;
    }

    data = new char[bytes.size()];
    for (int i = 0; i < bytes.size(); i++)
      data[i] = bytes[i];

    return bytes.size();
  }
};
#endif
