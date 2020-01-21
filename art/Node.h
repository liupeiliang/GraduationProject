#ifndef _Node_H
#define _Node_H

#include <bits/stdc++.h>

#define NODE4 0
#define NODE16 1
#define NODE48 2
#define NODE256 3

template <typename T> class Node{
public:
  Node();
  ~Node();

  virtual bool IsLeaf() const = 0 ;
  
private:
};


template <typename T>
Node<T>::Node()
{
}

template <typename T>
Node<T>::~Node()
{
}


#endif //_Node_H
