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

  T* Find(const char* key) const;
  void Insert(const char* key, T* value);
  ArtIterator SearchPrefix(const char* prefix);

private:

  LeafNode<T>* NewLeafNode(const char* key, int keyLen, T* value);
  int CheckPrefixPes(Node<T>* now, const char* key,
                     int keyLen, int depth) const;
  LeafNode<T>* MinLeaf(Node<T>* now);

  
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

    InnerNode<T>* now2 = (InnerNode<T>*)now;
    // 存在压缩前缀，进行匹配
    if (now2->mPrefixLen > 0) {

      // 查找时只对存储前缀进行乐观匹配即可
      int prefixLen = now2->CheckPrefixOpt(key, keyLen, depth);

      // 判断是否完全匹配
      if (prefixLen != min(now2->mPrefixLen, MAX_PREFIX_LEN))
        return nullptr;

      depth = depth + prefixLen;
    }

    child = now2->FindChild(key[depth]);
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

      LeafNode<T>* now1 = (LeafNode<T>*)now;
      //两种情况：完全匹配 or 部分匹配新建节点
      int pos = now1->MatchPoint(key, keyLen, depth);
      
      if (pos == -1) {
        // 完全匹配，此时替换原value
        now1->mValue = value;
        return;
      }

      // 部分匹配，新建分叉节点和叶节点
      Node4<T>* newParent = mNodeAllocator->NewNode(NODE4);
      LeafNode<T>* leafNode = NewLeafNode(key, keyLen, value);

      newParent->mPrefixLen = pos;
      memcpy(newParent->mKey, key+depth, min(MAX_PREFIX_LEN, pos));
      newParent->AddChild(key[depth+pos], leafNode);
      newParent->AddChild(now1->mKey[depth+pos], now1);

      BARRIER();

      // 最后修改父节点指针
      *now = newParent;
      return;
    }

    // 此时now必定为InnerNode
    InnerNode<T>* now2 = (InnerNode<T>*)now;
    // 如果now有prefix
    if (now2->mPrefixLen > 0) {
      
      // 插入时需要做悲观的完全匹配
      int pos = CheckPrefixPes(now, key, keyLen, depth);
      
      if (pos >= now2->mPrefixLen) {
        // 全部匹配，继续向下
        depth += now2->mPrefixLen;
        child = now2->FindChild(key[depth]);
        
        if (child == nullptr) {
          // 没有对应向下节点，新建叶节点，直接插入
          LeafNode<T>* l = NewLeafNode(key, keyLen, value);
          // 若插入时发现节点已满，需要扩张
          if (now2->IsFull()) {
//            InnerNode<T>* newNow = Grow(now2);
          }
          else now2->AddChild(key[depth], l);
          return;
        }

        ++depth;
        now = child;
        continue;
      }

      // else {
      // 未全部匹配，需新建节点
      
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


template <typename T>
int Art<T>::CheckPrefixPes(Node<T>* now, const char* key,
                           int keyLen, int depth) const
{
  InnerNode<T>* now2 = (InnerNode<T>*)now;
  // 先判断当前节点已存前缀是否匹配
  int mx = std::min((int)std::min((uint8_t)MAX_PREFIX_LEN,
                                  now2->mPrefixLen),
                    keyLen - depth);
  int i;
  for (i = 0; i < mx; i++) {
    if (key[depth+i] != now2->mPrefix[i])
      return i;
  }

  // 若已存前缀完全匹配，需要找到一个叶节点进行匹配
  if (now->mPrefixLen > MAX_PREFIX_LEN) {
    // TODO: check again
    LeafNode<T>* l = MinLeaf(now);
    mx = std::min(l->mKeyLen, keyLen) - depth;
    for (; i < mx; i++) {
      if (l->key[depth+i] != key[depth+i])
        return i;
    }
  }
  
  return i;
}

template <typename T>
LeafNode<T>* Art<T>::MinLeaf(Node<T>* now)
{
  if (now == nullptr) return nullptr;
  if (now->IsLeaf()) return (LeafNode<T>*)now;

  return MinLeaf( ((InnerNode<T>*)now)->MinChild() );
}

#endif //_Art_H
