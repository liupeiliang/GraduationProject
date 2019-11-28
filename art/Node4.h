#ifndef _Node4_H
#define _Node4_H

#include "InnerNode.h"
#include "Node.h"

#pragma pack(4)

template <typename T>
class Node4 : public InnerNode<T>{
  
public:
  Node4();
  ~Node4();

private:
  
  Node<T>* mChildren[4];
  uint8_t mKey[4];
    
};

#endif //_Node4_H
