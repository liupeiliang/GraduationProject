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

  Node<T>* NewNode(uint8_t nodeType);
  void DeleteNode(Node<T>* now);
  
  void PushInGCqueue(Node<T>* now, int version);
  void GarbageCollection();
  shared_ptr<int> AcquireVersion();
  void AddVersion();
  
public:
  MemoryPool< Node4<T> > mPool4;
  MemoryPool< Node16<T> > mPool16;
  MemoryPool< Node48<T> > mPool48;
  MemoryPool< Node256<T> > mPool256;

  atomic<int> mVersion;
  shared_ptr<int> mVersionPointer;
  queue< Node<T>* > mGCqueue;
  queue< int > mVersionQueue;
  queue< shared_ptr<int> > mReaderQueue;
};

template <typename T>
NodeAllocator<T>::NodeAllocator()
{
  mVersion = 1;
  mVersionPointer = make_shared<int>(mVersion);
}

template <typename T>
NodeAllocator<T>::~NodeAllocator()
{
}

template <typename T>
Node<T>* NodeAllocator<T>::NewNode(uint8_t nodeType)
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
void NodeAllocator<T>::DeleteNode(Node<T>* now)
{
  switch (now->mNodeType) {
  case NODE4: mPool4.deleteElement((Node4<T>*)now); break;
  case NODE16: mPool16.deleteElement((Node16<T>*)now); break;
  case NODE48: mPool48.deleteElement((Node48<T>*)now); break;
  case NODE256:
    mPool256.deleteElement((Node256<T>*)now); break;
  case LEAFNODE: free((LeafNode<T>*)now); break;
  default:
    throw std::runtime_error("NodeAllocator::DeleteNode()");
  } 
}

template <typename T>
void NodeAllocator<T>::PushInGCqueue(Node<T>* now,
                                     int version)
{
  mGCqueue.push(now);
  mVersionQueue.push(version);
}

template <typename T>
void NodeAllocator<T>::GarbageCollection()
{
  int s = mGCqueue.size();

  //待回收节点达到一定规模再进行回收
  if (s < 50) return; 
  
  int mxDeleteVersion = mVersion-1;
  
  while (!mReaderQueue.empty()) {
    shared_ptr<int> sp = mReaderQueue.front();
    if (*(sp.get()) == mVersion) break;
    if (sp.use_count() == 2) { //注意这里sp也持有，所以为2
      mxDeleteVersion = *(sp.get());
      mReaderQueue.pop();
    }
    else {
      mxDeleteVersion = (*sp.get())-1;
      break;
    }
  }
  
  while (!mGCqueue.empty()) {
    int v = mVersionQueue.front();
    if (v <= mxDeleteVersion) {
      Node<T>* now = mGCqueue.front();
      mGCqueue.pop();
      mVersionQueue.pop();
      DeleteNode(now);
    }
    else break;
  }
  
}

template <typename T>
shared_ptr<int> NodeAllocator<T>::AcquireVersion()
{
  mReaderQueue.push(mVersionPointer);
  return mVersionPointer;
}

template <typename T>
void NodeAllocator<T>::AddVersion()
{
  mVersion++;
  mVersionPointer = make_shared<int>(mVersion);
}

#endif //_NodeAllocator_H
