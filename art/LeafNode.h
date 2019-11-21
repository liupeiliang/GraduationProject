#ifndef _LeafNode_H
#define _LeafNode_H

#include "Node.h"

template <typename T>
class LeafNode : public Node<T>{
public:
  LeafNode();
  ~LeafNode();

private:
};

#endif //_LeafNode_H
