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

  Node<T>* NewNode(const int nodeType) ;
  
private:
  MemoryPool< Node4<T> > mPool4;
  MemoryPool< Node16<T> > mPool16;
  MemoryPool< Node48<T> > mPool48;
  MemoryPool< Node256<T> > mPool256;
  
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
  case NODE4: return new(mPool4.allocate()) Node4<T>;
  case NODE16: return new(mPool16.allocate()) Node16<T>;
  case NODE48: return new(mPool48.allocate()) Node48<T>;
  case NODE256:return new(mPool256.allocate()) Node256<T>;
  default:
    throw std::runtime_error("NodeAllo NewNode");
  }
  
  return nullptr;
}


#endif //_NodeAllocator_H
