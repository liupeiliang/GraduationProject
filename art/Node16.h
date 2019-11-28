#ifndef _Node16_H
#define _Node16_H

#include "InnerNode.h"

template <typename T>
class Node16 : public InnerNode<T>{
  
public:
  Node16();
  ~Node16();

private:

  Node<T>* mChildren[16];
  uint8_t mKey[16];
  
};

#endif //_Node16_H
