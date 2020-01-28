#ifndef _Node256_H
#define _Node256_H

#include "InnerNode.h"

template <typename T>
class Node256 : public InnerNode<T>{
  
public:
  Node256();
  ~Node256();

  Node<T>** FindChild(const char partialKey) override ;
  void AddChild(char partialKey, Node<T>* child) override ;
  
public:

  Node<T>* mChildren[256];
  
};


template <typename T>
Node256<T>::Node256()
{
  memset(mChildren, 0, sizeof(mChildren));
}

template <typename T>
Node256<T>::~Node256()
{
}

template <typename T>
Node<T>** Node256<T>::FindChild(const char partialKey)
{
  return (mChildren[partialKey] == nullptr) ? nullptr : &mChildren[partialKey];
}

template <typename T>
void Node256<T>::AddChild(char partialKey, Node<T>* child)
{
  mChildren[partialKey + 128] = child;
  ++this->mChildrenNum;
}


#endif //_Node256_H
