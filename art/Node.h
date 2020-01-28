#ifndef _Node_H
#define _Node_H

#include <bits/stdc++.h>
#include <memory.h>
#include <cstddef>

#define NODE4 0
#define NODE16 1
#define NODE48 2
#define NODE256 3
#define LEAFNODE 4

#define MAX_PREFIX_LEN 4
#define BARRIER() __asm__ __volatile__("": : :"memory")

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
