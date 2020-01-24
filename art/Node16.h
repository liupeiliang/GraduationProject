#ifndef _Node16_H
#define _Node16_H

#include "InnerNode.h"

template <typename T>
class Node16 : public InnerNode<T>{
  
public:
  Node16();
  ~Node16();

  Node<T>** FindChild(const char partialKey) override;
  
public:

  Node<T>* mChildren[16];
  char mKey[16];
  
};


template <typename T>
Node16<T>::Node16()
{
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
  for (int i = 0; i < this->mChildrenNum; i++) {
    if(mKey[i] == partialKey)
      return &mChildren[i];
  }
  return nullptr;
}


#endif //_Node16_H
