#ifndef _NodeAllocator_H
#define _NodeAllocator_H

#include <bits/stdc++.h>
#include "MemoryPool.h"
#include "Node.h"
#include "InnerNode.h"
#include "LeafNode.h"
#include "Node4.h"
#include "Node16.h"
#include "Node48.h"
#include "Node256.h"
using namespace std;

template <typename T>
class NodeAllocator{
  
public:
  
  NodeAllocator();
  ~NodeAllocator();

  Node<T>* NewNode(const int nodeType);
  void GC(InnerNode<T>* now);
  
private:
  MemoryPool< Node4<T> > mPool4;
  MemoryPool< Node16<T> > mPool16;
  MemoryPool< Node48<T> > mPool48;
  MemoryPool< Node256<T> > mPool256;

  queue<InnerNode<T>*> mGCqueue;
  
};


template <typename T>
NodeAllocator<T>::NodeAllocator()
{
}

template <typename T>
NodeAllocator<T>::~NodeAllocator()
{
}

template <typename T>
Node<T>* NodeAllocator<T>::NewNode(const int nodeType)
{
  switch(nodeType){
  case NODE4: return (Node<T>*)(mPool4.newElement());
  case NODE16: return (Node<T>*)(mPool16.newElement());
  case NODE48: return (Node<T>*)(mPool48.newElement());
  case NODE256:return (Node<T>*)(mPool256.newElement());
  default:
    throw std::runtime_error("NodeAllocator::NewNode wrongType");
  }
  
  return nullptr;
}

template <typename T>
void NodeAllocator<T>::GC(InnerNode<T>* now)
{
  mGCqueue.push(now);
}

#endif //_NodeAllocator_H
