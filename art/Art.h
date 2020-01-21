#ifndef _Art_H
#define _Art_H

#include "Node.h"
#include "NodeAllocator.h"
#include "ArtIterator.h"

template <typename T> class Art{
public:
  Art();
  ~Art();

  T* Find(const char* key) const ;
  void Insert(const char* key, T* value) ;
  ArtIterator SearchPrefix(const char* prefix) ;
  
private:
  
  Node<T>* mRoot;
  NodeAllocator<T>* mNodeAllocator;


  Node4<T>* mRootGenerator;

};

template <typename T>
Art<T>::Art()
{
  mNodeAllocator = new NodeAllocator<T>;
  mRootGenerator = mNodeAllocator->NewNode(NODE4);
  mRoot = mRootGenerator->mChildren[0];
}

template <typename T>
Art<T>::~Art()
{
}

template <typename T>
T* Art<T>::Find(const char* key) const
{
  // TODO
  
  return nullptr;
}

template <typename T>
void Art<T>::Insert(const char* key, T* value)
{
}

template <typename T>
ArtIterator SearchPrefix(const char* prefix)
{
}

#endif //_Art_H
