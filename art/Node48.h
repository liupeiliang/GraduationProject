#ifndef _Node48_H
#define _Node48_H

#include "InnerNode.h"

template <typename T>
class Node48 : public InnerNode<T>{
  
public:
  Node48();
  ~Node48();

  Node<T>** FindChild(const char partialKey) override ;
  void AddChild(char partialKey, Node<T>* child) override ;
  Node<T>* MinChild() override ;
  
public:

  Node<T>* mChildren[48];
  uint8_t mIndex[256];
  
};


template <typename T>
Node48<T>::Node48()
{
  memset(mIndex, 0, sizeof(mIndex));
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
  if (index > 0) return &mChildren[index];
  return nullptr;
}

template <typename T>
void Node48<T>::AddChild(char partialKey, Node<T>* child)
{
  mChildren[this->mChildrenNum] = child;

  BARRIER();

  mIndex[partialKey + 128] = this->mChildrenNum++;
}

template <typename T>
Node<T>* Node48<T>::MinChild()
{
  return mChildren[0];
}

#endif //_Node48_H
