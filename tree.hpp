#include "unihead.h"
#include "node.hpp"
#include "byte.hpp"

class HoffmanTree
{
private:
  char* Data;
  size_t Size;
  vector<HoffmanNode*> Nodes;
  HoffmanNode* CurrentLeastNode = nullptr;
  HoffmanNode* root;

  HoffmanNode* FindNode(char c)
  {
    for (int i = 0; i < Nodes.size(); i++)
      {
        if (Nodes[i]->Content[0] == c)
          return Nodes[i];
      }
      return nullptr;
  }

  vector<HoffmanNode*>::iterator IteratorOf(HoffmanNode* node)
  {
    for (auto i = Nodes.begin(); i != Nodes.end(); i++)
      {
        if (*i == node)
          return i;
      }
      return Nodes.end();
  }

  void CountByteFrequency()
  {
    int freqs[256];
    for (int i = 0; i < 256; i++)
      freqs[i] = 0;

    for (int i = 0; i < Size; i++)
    {
      freqs[(unsigned char)Data[i]]++;
    }

    for (int i = 0; i < 256; i++)
    {
      if (freqs[i] != 0)
        Nodes.push_back(new HoffmanNode(string(1, (char)i), freqs[i]));
    }
  }

public:
  HoffmanTree(char* data, size_t size)
  {
    Data = data;
    Size = size;


    CountByteFrequency();

  }

  HoffmanNode* Root()
  {
    return root;
  }

  HoffmanTree(vector<HoffmanNode*> nodes)
  {
    Size = 0;

    for (int i = 0; i < nodes.size(); i++)
    {
      Size += nodes[i]->Frequency;
    }

    Nodes = nodes;
  }


  HoffmanNode* AttachNodes(HoffmanNode* left, HoffmanNode* right)
  {
    if (!left && !right)
      return nullptr;
    HoffmanNode* parent = new HoffmanNode(left->Content + right->Content, left->Frequency + right->Frequency);
    parent->SetLeftChild(left);
    parent->SetRightChild(right);

    return parent;
  }

  HoffmanNode* BuildTree()
  {
    cout << "Building Tree\n";
    HoffmanNode *left, *right, *parent;

    do
    {
       left = NextLeastFrequentNode();
       right = NextLeastFrequentNode();

       parent = AttachNodes(left, right);

       if (parent != nullptr)
          Nodes.push_back(parent);
    } while (parent->Frequency != Size);

    root = parent;
    return parent;
  }

  HoffmanNode* NextLeastFrequentNode()
  {
    int prevleast = (CurrentLeastNode != nullptr) ? CurrentLeastNode->Frequency : INT_MIN;
    int least = INT_MAX;
    int leastindex = -1;
    for (int i = 0; i < Nodes.size(); i++)
    {
      if (Nodes[i]->Frequency <= least && CurrentLeastNode != Nodes[i])
      {
        least = Nodes[i]->Frequency;
        leastindex = i;
      }
    }
    if (leastindex == -1)
      return nullptr;
    CurrentLeastNode = Nodes[leastindex];
    Nodes.erase(Nodes.begin() + leastindex);
    return CurrentLeastNode;
  }

  vector<HoffmanNode*> GetDictionaryAsVector()
  {
    return Nodes;
  }

  HoffmanByte Encode(char c, vector<HoffmanByte>* bytes, bool verbose)
  {
    char enc = 0;
    int _size = 0;
    HoffmanByte bt;
    HoffmanNode* current = root;
    bool found = false;
    while (!current->IsLeaf())
    {
      found = false;
      if (current->LeftChild)
      {
        if (current->LeftChild->FindContent(c) != -1)
        {
          current = current->LeftChild;
          enc = enc << 1;
          _size++;
          found = true;
        }
      }
      if (current->RightChild && !found)
      {
        if (current->RightChild->FindContent(c) != -1)
        {
          current = current->RightChild;
          enc = enc << 1;
          enc++;
          _size++;
        }
      }

      if (_size > 8)
      {
        cout << "Byte chunked for byte " << (int)c << '\n';
      }
      if (_size == 8)
      {
        bt.Data = enc;
        bt.Size = _size;
        enc = 0;
        _size = 0;
        if (bytes)
          bytes->push_back(bt);
        if (verbose)
          cout << "Byte chunked " << (int)bt.Data << bt.Size << '\n';
      }
    }
    if (_size > 0)
    {
      bt.Data = enc;
      bt.Size = _size;
      if (bytes)
        bytes->push_back(bt);
    }
    return bt;
  }
};
