#ifndef _Node256_H
#define _Node256_H

#include "InnerNode.h"

template <typename T>
class Node256 : public InnerNode<T>{
  
public:
  Node256();
  ~Node256();

  Node<T>** FindChild(const char partialKey) override;
  void AddChild(char partialKey, Node<T>* child) override;
  Node<T>* MinChild() override;
  bool IsFull() override;
  int NodeType() override;
  void CopyNode(InnerNode<T>* now) override;
  
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
int Node256<T>::NodeType()
{
  return NODE256;
}

template <typename T>
void Node256<T>::CopyNode(InnerNode<T>* now)
{
  memcpy(now, this, sizeof(Node256<T>));
}

#endif //_Node256_H
