#ifndef _Art_H
#define _Art_H

#include "Node.h"
#include "NodeAllocator.h"

template <typename T> class Art{
public:
  Art();
  ~Art();

  T* Find(const char* key) const ;
  void Insert(const char* key, T* value) ;
  // ArtIterator SearchPrefix(const char* prefix) ;
  
private:

  NodeAllocator<T>* mNodeAllocator;
  
};

#endif //_Art_H
