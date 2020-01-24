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
  // incomplete
  Node<T>* now = mRoot;
  Node<T>** child;

  int depth = 0, keyLen = std::strlen(key) + 1;
  
  while (now != nullptr) {
    
    if (now->IsLeaf()) {
      if ((LeafNode<T>)now->LeafMatch(key))
        return (LeafNode<T>)now->mValue;
      else return nullptr;
    }

    if ((InnerNode<T>)now->mPrefixLen > 0) {
      
      int prefixLen =
        (InnerNode<T>)now->CheckPrefix(key, keyLen, depth);
      
      if (prefixLen !=
          min((InnerNode<T>)now->mPrefixLen, MAX_PREFIX_LEN))
        return nullptr;

      depth = depth + prefixLen;
    }

    child = (InnerNode<T>)now->FindChild(key[depth]);
    now = (child) ? (*child) : nullptr;
    depth++;
    
  }
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
