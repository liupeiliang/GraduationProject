#ifndef _Node_H
#define _Node_H

#include <bits/stdc++.h>

template <typename T> class Node{
public:
  Node();
  ~Node();

  virtual bool IsLeaf() const = 0 ;
  
private:
};

#endif //_Node_H
