#ifndef _Node48_H
#define _Node48_H

#include "InnerNode.h"

template <typename T>
class Node48 : public InnerNode<T>{
  
public:
  Node48();
  ~Node48();

  Node<T>** FindChild(char partialKey);
  void AddChild(char partialKey, Node<T>* child);
  Node<T>* MinChild();
  bool IsFull();
  void CopyNode(Node<T>* now);

  char MinPartialKey();
  short NextPartialKey(char partialKey);
  
public:
  Node<T>* mChildren[48];
  uint8_t mIndex[256];
  
};

template <typename T>
Node48<T>::Node48()
{
  this->mNodeType = NODE48;
  memset(mIndex, 48, sizeof(mIndex));
  memset(mChildren, 0, sizeof(mChildren));
}

template <typename T>
Node48<T>::~Node48()
{
}

template <typename T>
Node<T>** Node48<T>::FindChild(const char partialKey)
{
  uint8_t index = mIndex[partialKey + 128];
  if (index < 48) return &mChildren[index];
  return nullptr;
}

template <typename T>
void Node48<T>::AddChild(char partialKey, Node<T>* child)
{
  mChildren[this->mChildrenNum] = child;
  mIndex[partialKey + 128] = this->mChildrenNum++;
}

template <typename T>
Node<T>* Node48<T>::MinChild()
{
  for (int i = 0; i < 256; i++) {
    if (mIndex[i] < 48)
      return mChildren[mIndex[i]];
  }
}

template <typename T>
bool Node48<T>::IsFull()
{
  return (this->mChildrenNum == 48);
}

template <typename T>
void Node48<T>::CopyNode(Node<T>* now)
{
  memcpy(now, this, sizeof(Node48<T>));
}

template <typename T>
char Node48<T>::MinPartialKey()
{
  for (int i = 0; i < 256; i++) {
    if (mIndex[i] < 48)
      return (char)(i-128);
  }
}

template <typename T>
short Node48<T>::NextPartialKey(char partialKey)
{
  for (int i = partialKey+128+1; i < 256; i++) {
    if (mIndex[i] < 48)
      return i-128;
  }
  return NO_NEXT;
}

#endif //_Node48_H
