#include "unihead.h"
#include "node.hpp"

class Util
{
public:
  static vector<HoffmanNode*> ParseDictionary(char* data, size_t size)
  {
    vector<HoffmanNode*> v;
    HoffmanNode* temp;
    int tempfreq;
    for (int i = 0; i < size; i += 5)
    {
      temp = new HoffmanNode(string(1, data[i]), 0);
      for (int j = 1; j < 5; j++)
      {
        tempfreq = data[i + j];
        if (j != 1)
          temp->Frequency = ((unsigned int)temp->Frequency) >> 8;
        temp->Frequency += ((unsigned int)tempfreq) << 24;
      }
      v.push_back(temp);
    }
    return v;
  }
};
