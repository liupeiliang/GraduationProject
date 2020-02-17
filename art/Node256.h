#ifndef _Node256_H
#define _Node256_H

#include "InnerNode.h"

template <typename T>
class Node256 : public InnerNode<T>{
  
public:
  Node256();
  ~Node256();

  Node<T>** FindChild(char partialKey);
  void AddChild(char partialKey, Node<T>* child);
  Node<T>* MinChild();
  bool IsFull();
  void CopyNode(Node<T>* now);

  char MinPartialKey();
  short NextPartialKey(char partialKey);
  
public:

  Node<T>* mChildren[256];
  
};


template <typename T>
Node256<T>::Node256()
{
  this->mNodeType = NODE256;
  memset(mChildren, 0, sizeof(mChildren));
}

template <typename T>
Node256<T>::~Node256()
{
}

template <typename T>
Node<T>** Node256<T>::FindChild(const char partialKey)
{
  return (mChildren[partialKey+128] == nullptr) ? nullptr : &mChildren[partialKey+128];
}

template <typename T>
void Node256<T>::AddChild(char partialKey, Node<T>* child)
{
  mChildren[partialKey + 128] = child;
  ++this->mChildrenNum;
}

template <typename T>
Node<T>* Node256<T>::MinChild()
{
  for (int i = 0; i < 256; i++) {
    if (mChildren[i] != nullptr)
      return mChildren[i];
  }
  return nullptr;
}

template <typename T>
bool Node256<T>::IsFull()
{
  return (this->mChildrenNum == 256);
}

template <typename T>
void Node256<T>::CopyNode(Node<T>* now)
{
  memcpy(now, this, sizeof(Node256<T>));
}

template <typename T>
char Node256<T>::MinPartialKey()
{
  for (int i = 0; i < 256; i++) {
    if (mChildren[i])
      return (char)(i-128);
  }
}

template <typename T>
short Node256<T>::NextPartialKey(char partialKey)
{
  for (int i = partialKey+128+1; i < 256; i++) {
    if (mChildren[i])
      return i-128;
  }
  return NO_NEXT;
}


#endif //_Node256_H
