#ifndef _ArtIterator_H
#define _ArtIterator_H

#include "Node.h"
#include "Node4.h"
#include "Node16.h"
#include "Node48.h"
#include "Node256.h"
#include "LeafNode.h"

template <typename T>
class ArtIterator{
public:
  ArtIterator();
  ~ArtIterator();

  bool HasNext();
  void GoNext();
  char* GetKey();
  T* GetValue();
  void Init(Node<T>* now);
  
private:
  char MinPartialKey(Node<T>* now);
  short NextPartialKey(Node<T>* now, char partialKey);
  Node<T>** FindChild(Node<T>* now, char partialKey);
  
  vector< Node<T>* > mStack;
  vector<char> mPartialKey;
};

template <typename T>
ArtIterator<T>::ArtIterator()
{
}

template <typename T>
ArtIterator<T>::~ArtIterator()
{
}

template <typename T>
bool ArtIterator<T>::HasNext()
{
  int len = mStack.size();
  for (int i = 0; i < len-1; i++) {
    short x = NextPartialKey(mStack[i], mPartialKey[i]);
    if (x != NO_NEXT) return true;
  }
  return false;
}

template <typename T>
void ArtIterator<T>::GoNext()
{
  if (!HasNext()) return;
  mStack.pop_back();
  int len = mStack.size();
  for (int i = len-1; i >= 0; i--) {
    short x = NextPartialKey(mStack[i], mPartialKey[i]);
    if (x != NO_NEXT) {
      Node<T>** child = FindChild(mStack[i], (char)x);
      if (child == nullptr) {
        throw std::runtime_error("ArtIterator::GoNext()");
      }
      mPartialKey.pop_back();
      mPartialKey.push_back((char)x);
      Init(*child);
      return;
    }
    mStack.pop_back();
    mPartialKey.pop_back();
  }
}

template <typename T>
char* ArtIterator<T>::GetKey()
{
  Node<T>* l = *(--mStack.end());
  if (l->mNodeType != LEAFNODE)
    throw std::runtime_error("Artiterator::GetKey type error");
  return ((LeafNode<T>*)l)->mKey;
}

template <typename T>
T* ArtIterator<T>::GetValue()
{
  Node<T>* l = *(--mStack.end());
  if (l->mNodeType != LEAFNODE)
    throw std::runtime_error("Artiterator::GetValue type error");
  return ((LeafNode<T>*)l)->mValue;
}

template <typename T>
void ArtIterator<T>::Init(Node<T>* now)
{
  while (now != nullptr) {
    mStack.push_back(now);
    if (now->mNodeType == LEAFNODE) break;
    char c = MinPartialKey(now);
    mPartialKey.push_back(c);
    switch (now->mNodeType) {
    case NODE4: now = ((Node4<T>*)now)->MinChild(); break;
    case NODE16: now = ((Node16<T>*)now)->MinChild(); break;
    case NODE48: now = ((Node48<T>*)now)->MinChild(); break;
    case NODE256: now = ((Node256<T>*)now)->MinChild(); break;
    default:
      throw std::runtime_error("ArtIterator::Init() wrong type");
    }
  }
}


template <typename T>
char ArtIterator<T>::MinPartialKey(Node<T>* now)
{
  switch (now->mNodeType) {
  case NODE4: return ((Node4<T>*)now)->MinPartialKey();
  case NODE16: return ((Node16<T>*)now)->MinPartialKey();
  case NODE48: return ((Node48<T>*)now)->MinPartialKey();
  case NODE256: return ((Node256<T>*)now)->MinPartialKey();
  default:
    throw std::runtime_error("Art::MinPartialKey NodeType error");
  }
}

template <typename T>
short ArtIterator<T>::NextPartialKey(Node<T>* now, char partialKey)
{
  
  switch (now->mNodeType) {
  case NODE4: return ((Node4<T>*)now)->NextPartialKey(partialKey);
  case NODE16: return ((Node16<T>*)now)->NextPartialKey(partialKey);
  case NODE48: return ((Node48<T>*)now)->NextPartialKey(partialKey);
  case NODE256: return ((Node256<T>*)now)->NextPartialKey(partialKey);
  default:
    throw std::runtime_error("Art::NextPartialKey NodeType error");
  }
}

template <typename T>
Node<T>** ArtIterator<T>::FindChild(Node<T>* now, char partialKey)
{
  switch (now->mNodeType) {
  case NODE4: 
    return ((Node4<T>*)now)->FindChild(partialKey);
  case NODE16:
    return ((Node16<T>*)now)->FindChild(partialKey);
  case NODE48:
    return ((Node48<T>*)now)->FindChild(partialKey);
  case NODE256:
    return ((Node256<T>*)now)->FindChild(partialKey);
  default:
    throw std::runtime_error("Art::FindChild() wrong type");
  }
}


#endif //_ArtIterator_H
