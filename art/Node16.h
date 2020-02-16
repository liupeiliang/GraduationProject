#ifndef _Node16_H
#define _Node16_H

#include "InnerNode.h"

template <typename T>
class Node16 : public InnerNode<T>{
  
public:
  Node16();
  ~Node16();

  Node<T>** FindChild(char partialKey);
  void AddChild(char partialKey, Node<T>* child);
  Node<T>* MinChild();
  bool IsFull();
  void CopyNode(Node<T>* now);

  char MinPartialKey();
  char NextPartialKey(char partialKey);
  
public:

  Node<T>* mChildren[16];
  char mKey[16];
  
};


template <typename T>
Node16<T>::Node16()
{
  this->mNodeType = NODE16;
  memset(mKey, 0, sizeof(mKey));
  memset(mChildren, 0, sizeof(mChildren));
}

template <typename T>
Node16<T>::~Node16()
{
}

template <typename T>
Node<T>** Node16<T>::FindChild(const char partialKey)
{
  int i = std::lower_bound(mKey, mKey+this->mChildrenNum,
                           partialKey) - mKey;
  if(mKey[i] == partialKey) return &mChildren[i];
  return nullptr;
}

template <typename T>
void Node16<T>::AddChild(char partialKey, Node<T>* child)
{
  int i = std::upper_bound(mKey, mKey+this->mChildrenNum,
                           partialKey) - mKey;
  if (i < this->mChildrenNum) {
    memmove(mKey+i+1, mKey+i, this->mChildrenNum-i);
    memmove(mChildren+i+1, mChildren+i,
           (this->mChildrenNum-i)*sizeof(Node<T>*));
  }
  mKey[i] = partialKey;
  mChildren[i] = child;
  ++this->mChildrenNum;
}

template <typename T>
Node<T>* Node16<T>::MinChild()
{
  return mChildren[0];
}

template <typename T>
bool Node16<T>::IsFull()
{
  return (this->mChildrenNum == 16);
}

template <typename T>
void Node16<T>::CopyNode(Node<T>* now)
{
  memcpy(now, this, sizeof(Node16<T>));
}

template <typename T>
char Node16<T>::MinPartialKey()
{
  return mKey[0];
}

template <typename T>
char Node16<T>::NextPartialKey(char partialKey)
{
  return *upper_bound(mKey, mKey+this->mChildrenNum, partialKey);
}

#endif //_Node16_H
