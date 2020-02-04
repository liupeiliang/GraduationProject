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

  Node<T>** FindChild(const char partialKey) override;
  void AddChild(char partialKey, Node<T>* child) override;
  Node<T>* MinChild() override;
  bool IsFull() override;
  int NodeType() override;
  void CopyNode(InnerNode<T>* now) override;
  
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

template <typename T>
void Node4<T>::AddChild(char partialKey, Node<T>* child)
{
  // 注意多线程下可能存在的问题
  mKey[this->mChildrenNum] = partialKey;
  mChildren[this->mChildrenNum] = child;

  // 保证线程安全
  BARRIER();

  ++this->mChildrenNum;
  
}

template <typename T>
Node<T>* Node4<T>::MinChild()
{
  return mChildren[0];
}

template <typename T>
bool Node4<T>::IsFull()
{
  return (this->mChildrenNum == 4);
}

template <typename T>
int Node4<T>::NodeType()
{
  return NODE4;
}

template <typename T>
void Node4<T>::CopyNode(InnerNode<T>* now)
{
  memcpy(now, this, sizeof(Node4<T>));
}

#endif //_Node4_H
