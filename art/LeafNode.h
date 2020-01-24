#ifndef _LeafNode_H
#define _LeafNode_H

#include "Node.h"

template <typename T>
class LeafNode : public Node<T>{
public:
  LeafNode();
  ~LeafNode();

  bool IsLeaf() const override { return true; }
  bool LeafMatch(const char* key) const ;
    
private:

  T* mValue;
  char mKey[];
  int mKeyLen;
};


template <typename T>
LeafNode<T>::LeafNode()
{
}

template <typename T>
LeafNode<T>::~LeafNode()
{
}

template <typename T>
bool LeafNode<T>::LeafMatch(const char* key) const
{
  int keyLen = std::strlen(key);
  if (keyLen != mKeyLen) return false;
  for (int i = 0; i < keyLen; i++) {
    if (key[i] != mKey[i]) return false;
  }
  return true;
}


#endif //_LeafNode_H
