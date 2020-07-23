#include "unihead.h"
#ifndef HOFF_NODE
#define HOFF_NODE
class HoffmanNode
{
public:
  HoffmanNode* Parent = nullptr;
  HoffmanNode* LeftChild = nullptr;
  HoffmanNode* RightChild = nullptr;
  string Content;
  int Frequency;

  HoffmanNode(string content, int freq)
  {
    Content = content;
    Frequency = freq;
  }

  int FindContent(char c)
  {
    for (int i = 0; i <  Content.length(); i++)
    {
      if (Content[i] == c)
        return i;
    }
    return -1;
  }

  HoffmanNode* GetNode(int path)
  {
    return (path == 0) ? LeftChild : RightChild;
  }

  bool IsLeaf()
  {
    return !(LeftChild || RightChild);
  }

  void SetParent(HoffmanNode* parent)
  {
    Parent = parent;
  }

  void SetLeftChild(HoffmanNode* lc)
  {
    lc->SetParent(this);
    LeftChild = lc;
  }

  void SetRightChild(HoffmanNode* rc)
  {
    rc->SetParent(this);
    RightChild = rc;
  }
};
#endif
