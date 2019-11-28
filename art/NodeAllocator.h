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

template <typename T>
class NodeAllocator{
  
public:
  
  NodeAllocator();
  ~NodeAllocator();

private:

  MemoryPool< Node4<T> > mPool4;
  MemoryPool< Node16<T> > mPool16;
  MemoryPool< Node48<T> > mPool48;
  MemoryPool< Node256<T> > mPool256;
  
};

#endif //_NodeAllocator_H
