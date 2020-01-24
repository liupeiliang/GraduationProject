#ifndef _InnerNode_H
#define _InnerNode_H

#include "Node.h"

template <typename T>
class InnerNode : public Node<T>{
  
public:
  InnerNode();
  ~InnerNode();

  bool IsLeaf() const override { return false; }
  int CheckPrefix(const char* key, int keyLen, int depth) const;
  virtual Node<T>** FindChild(const char partialKey) = 0 ;
  
public:
  
  uint8_t mChildrenNum;
  uint8_t mPrefixLen;
  char mPrefix[MAX_PREFIX_LEN];

};


template <typename T>
InnerNode<T>::InnerNode()
{
  mChildrenNum = 0;
  mPrefixLen = 0;
  memset(mPrefix, 0, sizeof(mPrefix));
}

template <typename T>
InnerNode<T>::~InnerNode()
{
}

template <typename T>
int InnerNode<T>::CheckPrefix(const char* key, int keyLen, int depth) const
{
  int mx = std::min((int)std::min((uint8_t)MAX_PREFIX_LEN, mPrefixLen),
                    keyLen - depth);
  for (int i = 0; i < mx; i++) {
    if (key[depth+i] != mPrefix[i])
      return i;
  }
  return mx;
}

#endif //_InnerNode_H
