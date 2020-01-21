#ifndef _Node48_H
#define _Node48_H

#include "InnerNode.h"

template <typename T>
class Node48 : public InnerNode<T>{
  
public:
  Node48();
  ~Node48();

private:

  Node<T>* mChildren[48];
  uint8_t mKey[256];
  
};


template <typename T>
Node48<T>::Node48()
{
}

template <typename T>
Node48<T>::~Node48()
{
}


#endif //_Node48_H
