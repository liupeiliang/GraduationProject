#ifndef _Node4_H
#define _Node4_H

#include "InnerNode.h"
#include "Node.h"
using namespace std;

#pragma pack(8)

template <typename T>
class Node4 : public InnerNode<T>{
  
public:
  Node4();
  ~Node4();

  Node<T>** FindChild(const char partialKey) override ;
  
public:
  
  Node<T>* mChildren[4];
  char mKey[4];
    
};

template <typename T>
Node4<T>::Node4()
{
  memset(mKey, 0, sizeof(mKey));
  memset(mChildren, 0, sizeof(mChildren));
}

template <typename T>
Node4<T>::~Node4()
{
}

template <typename T>
Node<T>** Node4<T>::FindChild(const char partialKey)
{
  for (int i = 0; i < this->mChildrenNum; i++) {
    if(mKey[i] == partialKey)
      return &mChildren[i];
  }
  return nullptr;
}

#endif //_Node4_H
