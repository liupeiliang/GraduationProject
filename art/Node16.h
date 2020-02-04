#ifndef _Node16_H
#define _Node16_H

#include "InnerNode.h"

template <typename T>
class Node16 : public InnerNode<T>{
  
public:
  Node16();
  ~Node16();

  Node<T>** FindChild(const char partialKey) override;
  void AddChild(char partialKey, Node<T>* child) override;
  Node<T>* MinChild() override;
  bool IsFull() override;
  int NodeType() override;
  void CopyNode(InnerNode<T>* now) override;
  
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

template <typename T>
void Node16<T>::AddChild(char partialKey, Node<T>* child)
{
  // 同NODE4处理方法
  mKey[this->mChildrenNum] = partialKey;
  mChildren[this->mChildrenNum] = child;

  BARRIER();

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
int Node16<T>::NodeType()
{
  return NODE16;
}

template <typename T>
void Node16<T>::CopyNode(InnerNode<T>* now)
{
  memcpy(now, this, sizeof(Node16<T>));
}

#endif //_Node16_H
