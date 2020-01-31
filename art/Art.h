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
  InnerNode<T>* Grow(InnerNode<T>* now);
  InnerNode<T>* CopyNode(InnerNode<T>* now);
  
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
  // TODO: 等待检查2nd
  
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

      depth = depth + now2->mPrefixLen;
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
  // TODO: 1. 等待全面检查
  //       2. 注意n和child为Node<T>**
  
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

      LeafNode<T>* now1 = (LeafNode<T>*)(*now);
      //两种情况：完全匹配 or 部分匹配新建节点
      int pos = now1->MatchPoint(key, keyLen, depth);
      
      if (pos == -1) {
        // 完全匹配，此时替换原value
        /* exact match:
         * => "replace"
         * => replace value of current node.
         * => return old value to caller to handle.
         *        _                             _
         *        |                             |
         *       (aa)                          (aa)
         *    a /    \ b     +[aaaaa,v3]    a /    \ b
         *     /      \      ==========>     /      \
         * *(aa)->v1  ()->v2             *(aa)->v3  ()->v2
         *
         * from rafaelkallis
         */
        now1->mValue = value;
        return;
      }

      // 部分匹配，新建分叉节点和叶节点
      Node4<T>* newNode = mNodeAllocator->NewNode(NODE4);
      LeafNode<T>* leafNode = NewLeafNode(key, keyLen, value);

      newNode->mPrefixLen = pos;
      memcpy(newNode->mKey, key+depth, min(MAX_PREFIX_LEN, pos));
      newNode->AddChild(key[depth+pos], leafNode);
      newNode->AddChild(now1->mKey[depth+pos], now1);

      // 最后修改父节点指针
      BARRIER();
      *now = newNode;
      return;
    }

    // 此时now必定为InnerNode
    InnerNode<T>* now2 = (InnerNode<T>*)(*now);
    // 如果now有prefix
    if (now2->mPrefixLen > 0) {
      
      // 插入时需要做悲观的完全匹配
      int pos = CheckPrefixPes(*now, key, keyLen, depth);
      
      if (pos >= now2->mPrefixLen) {
        // 全部匹配，继续向下
        depth += now2->mPrefixLen;
        child = now2->FindChild(key[depth]);
        
        if (child == nullptr) {
          // 没有对应向下节点，新建叶节点，直接插入
          /*
           * no child associated with the next partial key.
           * => create new node with value to insert.
           * => new node becomes current node's child.
           *
           *      *(aa)->Ø              *(aa)->Ø
           *    a /        +[aab,v2]  a /    \ b
           *     /         ========>   /      \
           *   (a)->v1               (a)->v1 +()->v2
           *
           * from rafaelkallis
           */
          LeafNode<T>* l = NewLeafNode(key, keyLen, value);
          // 若插入时发现节点已满，需要扩张
          if (now2->IsFull()) {
            InnerNode<T>* newNode = Grow(now2);
            newNode->AddChild(key[depth], l);

            BARRIER();
            *now = newNode;

            BARRIER();
            mNodeAllocator->GC(now2);
            
          }
          else now2->AddChild(key[depth], l);
          return;
        }

        /* propagate down and repeat:
         *
         *     *(aa)->Ø                   (aa)->Ø
         *   a /    \ b    +[aaba,v3]  a /    \ b     repeat
         *    /      \     =========>   /      \     ========>  ...
         *  (a)->v1  ()->v2           (a)->v1 *()->v2
         *
         * from rafaelkallis
         */
        ++depth;
        now = child;
        continue;
      }

      // else {
      // 未全部匹配，需新建节点
      /* prefix mismatch:
       * => new parent node with common prefix and no associated value.
       * => new node with value to insert.
       * => current and new node become children of new parent node.
       *
       *        |                        |
       *      *(aa)                    +(a)->Ø
       *    a /    \ b     +[ab,v3]  a /   \ b
       *     /      \      =======>   /     \
       *  (aa)->v1  ()->v2          *()->Ø +()->v3
       *                          a /   \ b
       *                           /     \
       *                        (aa)->v1 ()->v2
       *                        /|\      /|\
       *
       * from rafaelkallis
       */
      Node4<T>* newNode = mNodeAllocator->NewNode(NODE4);
      LeafNode<T>* leafNode = NewLeafNode(key, keyLen, value);

      newNode->mPrefixLen = pos;
      memcpy(newNode->mPrefix, now2->mPrefix, min(MAX_PREFIX_LEN, pos));
      newNode->AddChild(key[depth+pos], leafNode);

      // 需要调整原来节点的prefix，此时CopyOnWrite，复制原节点修改
      InnerNode<T>* copyNode = CopyNode(now2);
      copyNode->mPrefixLen -= pos+1;
      newNode->AddChild(key[depth+pos], copyNode);
      
      if (now2->mPrefixLen <= MAX_PREFIX_LEN) {
        // 所有信息已知，直接修改prefix
        memmove(copyNode->mPrefix, copyNode->mPrefix + pos + 1,
                std::min(MAX_PREFIX_LEN, copyNode->mPrefixLen) );  
      } else {
        // 此时存在未知信息，需要从MinLeaf找到未知前缀部分
        LeafNode<T>* l = MinLeaf(*now);
        memcpy(copyNode->mPrefix, l->mKey + depth + pos + 1,
               std::min(MAX_PREFIX_LEN, copyNode->mPrefixLen) );
      }

      // 最后修改父节点指针以保证线程安全
      BARRIER();
      *now = newNode;

      return;
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

template <typename T>
InnerNode<T>* Art<T>::Grow(InnerNode<T>* now)
{
  switch (now->NodeType()) {
    
  case (NODE4): 
    Node4<T>* now1 = (Node4<T>*)now;
    Node16<T>* newNode = mNodeAllocator->NewNode(NODE16);
    
    newNode->mPrefixLen = now1->mPrefixLen;
    newNode->mChildrenNum = now1->mChildrenNum;
    memcpy(newNode->mPrefix, now1->mPrefix, MAX_PREFIX_LEN);
    memcpy(newNode->mChildren, now1->mChildren,
           sizeof(Node<T>*) * now1->mChildrenNum);
    memcpy(newNode->mKey, now1->mKey, now1->mChildrenNum);
    return (InnerNode<T>*)newNode;

    
  case (NODE16):
    Node16<T>* now2 = (Node16<T>*)now;
    Node48<T>* newNode2 = mNodeAllocator->NewNode(NODE48);

    newNode2->mPrefixLen = now2->mPrefixLen;
    newNode2->mChildrenNum = now2->mChildrenNum;
    memcpy(newNode2->mPrefix, now2->mPrefix, MAX_PREFIX_LEN);
    memcpy(newNode2->mChildren, now2->mChildren,
           sizeof(Node<T>*) * now2->mChildrenNum);
    for (int i = 0; i < now2->mChildrenNum; i++) {
      newNode2->mIndex[now2->mKey[i] + 128] = i;
    }
    return (InnerNode<T>*)newNode2;

    
  case (NODE48):
    Node48<T>* now3 = (Node48<T>*)now;
    Node256<T>* newNode3 = mNodeAllocator->NewNode(NODE256);

    newNode3->mPrefixLen = now3->mPrefixLen;
    newNode3->mChildrenNum = now3->mChildrenNum;
    memcpy(newNode3->mPrefix, now3->mPrefix, MAX_PREFIX_LEN);
    for (int i = 0; i < 256; i++) {
      if (now3->mIndex[i] != 48) {
        newNode3->mChildren[i] = now3->mChildren[now3->mIndex[i]];
      }
    }
    return (InnerNode<T>*)newNode3;

    
  case (NODE256):
    throw std::runtime_error("Art::Grow(): Grow NODE256!");
    break;

  default:
    throw std::runtime_error("Art::Grow(): NodeType error");

  }
}

template <typename T>
InnerNode<T>* Art<T>::CopyNode(InnerNode<T>* now)
{
  InnerNode<T>* newNode = mNodeAllocator->NewNode(now->NodeType());
  (&newNode) = (&now);
  return newNode;
}

#endif //_Art_H
