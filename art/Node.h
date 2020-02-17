#ifndef _Node_H
#define _Node_H

#include <bits/stdc++.h>
#include <memory.h>
#include <cstddef>

#define NODE4 (uint8_t)1
#define NODE16 (uint8_t)2
#define NODE48 (uint8_t)3
#define NODE256 (uint8_t)4
#define LEAFNODE (uint8_t)5

#define MAX_PREFIX_LEN (int)10
#define NO_NEXT (short)256
#define BARRIER() __asm__ __volatile__("": : :"memory")

#pragma pack(1)

template <typename T> class Node{
public:
  Node();
  ~Node();

public:
  uint8_t mNodeType;

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
