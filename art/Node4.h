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

  Node<T>** FindChild(const char partialKey);
  void AddChild(char partialKey, Node<T>* child);
  Node<T>* MinChild();
  bool IsFull();
  void CopyNode(Node<T>* now);
  
public:
  
  Node<T>* mChildren[4];
  char mKey[4];
    
};

template <typename T>
Node4<T>::Node4()
{
  this->mNodeType = NODE4;
  memset(mKey, 0, sizeof(mKey));
  memset(mChildren, 0, sizeof(mChildren));
}

template <typename T>
Node4<T>::~Node4()
{
}

template <typename T>
Node<T>** Node4<T>::FindChild(char partialKey)
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
  int i;
  for (i = 0; i < this->mChildrenNum; i++) {
    if (mKey[i] > partialKey) {
        memmove(mKey+i+1, mKey+i, this->mChildrenNum-i);
        memmove(mChildren+i+1, mChildren+i,
                (this->mChildrenNum-i)*sizeof(Node<T>*));
        break;
    }
  }
  mKey[i] = partialKey;
  mChildren[i] = child;
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
void Node4<T>::CopyNode(Node<T>* now)
{
  memcpy(now, this, sizeof(Node4<T>));
}

#endif //_Node4_H
