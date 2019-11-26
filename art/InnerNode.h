#ifndef _InnerNode_H
#define _InnerNode_H

#include "Node.h"

template <typename T>
class InnerNode : public Node<T>{
  
public:
  InnerNode();
  ~InnerNode();

  bool IsLeaf() const override { return false; }
  
private:
  
  uint8_t mChildrenNum;
  uint8_t mPrefixLen;
  uint8_t mPrefix[6];

};

#endif //_InnerNode_H
