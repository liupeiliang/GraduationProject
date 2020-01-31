#ifndef _LeafNode_H
#define _LeafNode_H

#include "Node.h"

#pragma pack(4)

template <typename T>
class LeafNode : public Node<T>{
public:
  LeafNode();
  ~LeafNode();

  bool IsLeaf() const override { return true; }
  bool LeafMatch(const char* key, int keyLen) const ;
  int MatchPoint(const char* key, int keyLen, int depth) const;
  
private:

  T* mValue;
  int mKeyLen;
  char mKey[];
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
bool LeafNode<T>::LeafMatch(const char* key, int keyLen) const
{
  if (keyLen != mKeyLen) return false;
  for (int i = 0; i < keyLen; i++) {
    if (key[i] != mKey[i]) return false;
  }
  return true;
}

template <typename T>
int LeafNode<T>::MatchPoint(const char* key, int keyLen, int depth) const
{
  int mx = std::min(keyLen, mKeyLen);
  for (int i = depth; i < mx; i++) {
    if (key[i] != mKey[i]) return i;
  }
  if (keyLen == mKeyLen) return -1;
  // 此时一个串是另一个的前缀，理论上该情况不可能出现
  // else return Left + 1;
  throw std::runtime_error("LeafNode::MatchPoint(): prefix error");
}

#endif //_LeafNode_H
