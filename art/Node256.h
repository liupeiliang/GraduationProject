#ifndef _Node256_H
#define _Node256_H

#include "InnerNode.h"

template <typename T>
class Node256 : public InnerNode<T>{
  
public:
  Node256();
  ~Node256();

private:

  Node<T>* mChildren[256];
  
};


template <typename T>
Node256<T>::Node256()
{
}

template <typename T>
Node256<T>::~Node256()
{
}


#endif //_Node256_H
