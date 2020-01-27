#ifndef _Art_H
#define _Art_H

#include "Node.h"
#include "NodeAllocator.h"
#include "ArtIterator.h"
#include "LeafNode.h"

template <typename T> class Art{
public:
  Art();
  ~Art();

  T* Find(const char* key) const ;
  void Insert(const char* key, T* value) ;
  ArtIterator SearchPrefix(const char* prefix) ;

private:

  LeafNode<T>* NewLeafNode(const char* key, int keyLen, T* value) ;
  
private:
  
  Node<T>* mRoot;
  Node<T>* mRootGenerator;
  NodeAllocator<T>* mNodeAllocator;

};

template <typename T>
Art<T>::Art()
{
  mNodeAllocator = new NodeAllocator<T>;
  mRootGenerator = mNodeAllocator->NewNode(NODE4);
  mRoot = ((Node4<T>*)mRootGenerator)->mChildren[0];
}

template <typename T>
Art<T>::~Art()
{
  delete mNodeAllocator;
}

template <typename T>
T* Art<T>::Find(const char* key) const
{
  // incomplete
  Node<T>* now = mRoot;
  Node<T>** child;

  int depth = 0, keyLen = std::strlen(key) + 1;
  
  while (now != nullptr) {

    // 当前节点为叶节点，完全匹配 / 部分匹配
    if (now->IsLeaf()) {
      if (((LeafNode<T>*)now)->LeafMatch(key, keyLen))
        return ((LeafNode<T>*)now)->mValue;
      else return nullptr;
    }

    // 存在压缩前缀，进行匹配
    if (((InnerNode<T>*)now)->mPrefixLen > 0) {
      
      int prefixLen =
        ((InnerNode<T>*)now)->CheckPrefix(key, keyLen, depth);

      // 判断是否完全匹配
      if (prefixLen !=
          min(((InnerNode<T>*)now)->mPrefixLen, MAX_PREFIX_LEN))
        return nullptr;

      depth = depth + prefixLen;
    }

    child = ((InnerNode<T>*)now)->FindChild(key[depth]);
    now = (child) ? (*child) : nullptr;
    depth++; // 走到child边上还有一个字符
    
  }
  return nullptr;
}

template <typename T>
void Art<T>::Insert(const char* key, T* value)
{
  int keyLen = strlen(key) + 1, depth = 0;

  // 根节点为null即第一次插入
  if (mRoot == nullptr) {
    mRoot = (Node<T>*)NewLeafNode(key, keyLen, value);
    return;
  }

  Node<T>** now = &mRoot;
  Node<T>** child;

  while (true) {

    
    // 如果当前节点到达节点为叶节点
    if (now->IsLeaf()) {
      
      //两种情况：完全匹配 or 部分匹配新建节点
      int pos = ((LeafNode<T>*)now)->MatchPoint(key, keyLen, depth);
      
      if (pos == -1) {
        // 完全匹配，此时替换原value
        ((LeafNode<T>*)now)->mValue = value;
        return;
      }

      // 部分匹配，新建分叉节点和两个叶节点
      
    }
    
  }
}

template <typename T>
ArtIterator Art<T>::SearchPrefix(const char* prefix)
{
  ArtIterator tem;
  return tem;
}

template <typename T>
LeafNode<T>* Art<T>::NewLeafNode(const char* key, int keyLen, T* value)
{
  LeafNode<T>* tem = (LeafNode<T>*)malloc(sizeof(LeafNode<T>) + keyLen);
  tem->mKeyLen = keyLen;
  tem->mValue = value;
  memcpy(tem->mKey, key, keyLen);
  return nullptr;
}

#endif //_Art_H
