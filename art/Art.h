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

  T* Find(const char* key);
  void Insert(const char* key, T* value);
  ArtIterator<T>* SearchPrefix(const char* prefix);
  
private:
  LeafNode<T>* NewLeafNode(const char* key, int keyLen, T* value);
  int CheckPrefixPes(Node<T>* now, const char* key,
                     int keyLen, int depth);
  LeafNode<T>* MinLeaf(Node<T>* now);
  InnerNode<T>* Grow(InnerNode<T>* now);
  
  Node<T>** FindChild(Node<T>* now, char partialKey);
  void AddChild(Node<T>* now, char partialKey, Node<T>* child);
  Node<T>* MinChild(Node<T>* now);
  bool IsFull(Node<T>* now);
  Node<T>* CopyNode(Node<T>* now);

private:
  Node<T>* mRoot;
  Node4<T>* mRootGenerator;
  NodeAllocator<T>* mNodeAllocator;
};

template <typename T>
Art<T>::Art()
{
  mNodeAllocator = new NodeAllocator<T>;
  mRootGenerator = (Node4<T>*)mNodeAllocator->NewNode(NODE4);
  mRoot = mRootGenerator->mChildren[0];
  // 注意这里需要保证mRoot的原子性，故使用了mRootGenerator
}

template <typename T>
Art<T>::~Art()
{
  // 回收所有叶节点
  queue<Node<T>*> q;
  if (mRoot != nullptr) q.push(mRoot);

  while (!q.empty()) {
    
    Node<T>* now = q.front();
    q.pop();
    if (now == nullptr) continue;
    
    if (now->mNodeType == LEAFNODE) {
      free((LeafNode<T>*)now);
    } else {
      InnerNode<T>* now2;
      now2 = (InnerNode<T>*)now;
      int cn = now2->mChildrenNum;
      
      switch (now->mNodeType) {
      case (NODE4): {
        for (int i = 0; i < cn; i++) 
          q.push(((Node4<T>*)now)->mChildren[i]);
        break;
      }
      case (NODE16): {
        for (int i = 0; i < cn; i++) 
          q.push(((Node16<T>*)now)->mChildren[i]);
        break;
      }
      case (NODE48): {
        for (int i = 0; i < cn; i++) 
          q.push(((Node48<T>*)now)->mChildren[i]);
        break;
      }
      case (NODE256): {
        for (int i = 0; i < 256; i++) 
          if (((Node256<T>*)now)->mChildren[i] != nullptr)
            q.push(((Node256<T>*)now)->mChildren[i]);
        break;
      }
      default: {
        std::cout << "~Art() NodeType error" << std::endl;
      }
      }
    }
  }
  // 直接通过回收内存池回收所有中间结点
  delete mNodeAllocator;
}

template <typename T>
T* Art<T>::Find(const char* key)
{
  shared_ptr<int> sp = mNodeAllocator->AcquireVersion();
  
  Node<T>* now = mRoot;
  Node<T>** child;
  int depth = 0, keyLen = std::strlen(key) + 1;
  
  while (now != nullptr) {

    // 当前节点为叶节点，完全匹配 / 部分匹配
    if (now->mNodeType == LEAFNODE) {
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
      if (prefixLen != std::min(now2->mPrefixLen, MAX_PREFIX_LEN))
        return nullptr;

      depth = depth + now2->mPrefixLen;
    }

    child = FindChild(now, key[depth]);
    now = (child) ? (*child) : nullptr;
    depth++; // 走到child边上还有一个字符
    
  }
  return nullptr;
}

template <typename T>
void Art<T>::Insert(const char* key, T* value)
{  
  // 写线程触发垃圾回收
  mNodeAllocator->GarbageCollection();
  
  int keyLen = strlen(key) + 1, depth = 0;

  // 根节点为null即第一次插入
  if (mRoot == nullptr) {
    mRoot = (Node<T>*)NewLeafNode(key, keyLen, value);
    return;
  }

  Node<T>** now = &mRoot;
  Node<T>** child;
  Node<T>* old = nullptr;
  
  while (true) {

    // 如果当前节点到达节点为叶节点
    if ((*now)->mNodeType == LEAFNODE) {
      
      LeafNode<T>* now1 = (LeafNode<T>*)(*now);
      // 两种情况：完全匹配 or 部分匹配新建节点
      int pos = now1->MatchPoint(key, keyLen, depth);
      
      if (pos == -1) {
        // 完全匹配，此时替换原value
        /* exact match:
         * => "replace"
         * => replace value of current node.
         *        _                             _
         *        |                             |
         *       (aa)                          (aa)
         *    a /    \ b     +[aaaaa,v3]    a /    \ b
         *     /      \      ==========>     /      \
         * *(aa)->v1  ()->v2             *(aa)->v3  ()->v2
         *
         * graph from rafaelkallis
         */
        LeafNode<T>* l = NewLeafNode(now1->mKey,
                                     now1->mKeyLen, value);
        
        BARRIER();
        old = *now;
        *now = l;
        BARRIER();
        int version = (int)(mNodeAllocator->mVersion);
        mNodeAllocator->PushInGCqueue(old, version);
        mNodeAllocator->AddVersion();
        return;
      }

      // 部分匹配，新建分叉节点和叶节点
      Node4<T>* newNode = (Node4<T>*)mNodeAllocator->NewNode(NODE4);
      LeafNode<T>* leafNode = NewLeafNode(key, keyLen, value);

      newNode->mPrefixLen = pos-depth;
      memcpy(newNode->mPrefix, key+depth,
             std::min(MAX_PREFIX_LEN, newNode->mPrefixLen));
      newNode->AddChild(key[pos], leafNode);
      newNode->AddChild(now1->mKey[pos], now1);

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
      int len = CheckPrefixPes(*now, key, keyLen, depth);
      
      if (len >= now2->mPrefixLen) {
        // 全部匹配，继续向下
        depth += now2->mPrefixLen;
      } else {
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
        Node4<T>* newNode = (Node4<T>*)mNodeAllocator->NewNode(NODE4);
        LeafNode<T>* leafNode = NewLeafNode(key, keyLen, value);

        newNode->mPrefixLen = len;
        memcpy(newNode->mPrefix, now2->mPrefix,
               std::min(MAX_PREFIX_LEN, len));
        newNode->AddChild(key[depth+len], leafNode);

        // 需要调整原来节点的prefix，此时CopyOnWrite，复制原节点修改
        // TODO: 不用复制prefix部分
        InnerNode<T>* copyNode = (InnerNode<T>*)CopyNode(*now);
        copyNode->mPrefixLen -= len+1;
      
        if (now2->mPrefixLen <= MAX_PREFIX_LEN) {
          // 所有信息已知，直接修改prefix
          newNode->AddChild(now2->mPrefix[len], copyNode);
          memmove(copyNode->mPrefix, copyNode->mPrefix + len + 1,
                  std::min(MAX_PREFIX_LEN, copyNode->mPrefixLen) );  
        } else {
          // 此时存在未知信息，需要从MinLeaf找到未知前缀部分
          LeafNode<T>* l = MinLeaf(*now);
          newNode->AddChild(l->mKey[depth+len], copyNode);
          memcpy(copyNode->mPrefix, l->mKey + depth + len + 1,
                 std::min(MAX_PREFIX_LEN, copyNode->mPrefixLen) );
        }

        // 最后修改父节点指针以保证线程安全
        BARRIER();
        old = *now;
        *now = newNode;
        BARRIER();
        int version = (int)(mNodeAllocator->mVersion);
        mNodeAllocator->PushInGCqueue(old, version);
        mNodeAllocator->AddVersion();
        return;
      }
    }

    child = FindChild(*now, key[depth]);
        
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
      Node<T>* newNode;
      // 若插入时发现节点已满，需要扩张
      if (IsFull(*now)) {
        newNode = (Node<T>*)Grow(now2);
      }
      else newNode = CopyNode(*now);
      AddChild(newNode, key[depth], l);
      
      BARRIER();
      old = *now;
      *now = newNode;
      BARRIER();
      int version = (int)(mNodeAllocator->mVersion);
      mNodeAllocator->PushInGCqueue(old, version);
      mNodeAllocator->AddVersion();
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
    
  } // while(){}
}

template <typename T>
ArtIterator<T>* Art<T>::SearchPrefix(const char* prefix)
{
  shared_ptr<int> sp = mNodeAllocator->AcquireVersion();
  
  ArtIterator<T>* it = new ArtIterator<T>;
  Node<T>* now = mRoot;
  Node<T>** child;

  int depth = 0, prefixLen = std::strlen(prefix);
  
  while (now != nullptr) {

    if (depth == prefixLen) {
      it->Init(now);
      return it;
    }

    // 当前节点为叶节点
    if (now->mNodeType == LEAFNODE) {
      int pos = ((LeafNode<T>*)now)->
        MatchPoint(prefix, prefixLen, depth);
      if (pos == prefixLen) {
        it->Init(now);
        return it;
      }
      else return nullptr;
    }

    InnerNode<T>* now2 = (InnerNode<T>*)now;
    // 存在压缩前缀，进行匹配
    if (now2->mPrefixLen > 0) {

      int len = CheckPrefixPes(now, prefix, prefixLen, depth);
      // 前缀匹配成功
      if (depth+len == prefixLen) {
        it->Init(now);
        return it;
      }
      // 判断是否继续下降
      if (len != now2->mPrefixLen)
        return nullptr;

      depth = depth + now2->mPrefixLen;
    }

    child = FindChild(now, prefix[depth]);
    now = (child) ? (*child) : nullptr;
    depth++; // 走到child边上还有一个字符
    
  }
  
  return nullptr;
}

template <typename T>
LeafNode<T>* Art<T>::NewLeafNode(const char* key, int keyLen, T* value)
{
  LeafNode<T>* tem = new(malloc(sizeof(LeafNode<T>) + keyLen)) LeafNode<T>;
  tem->mKeyLen = keyLen;
  tem->mValue = value;
  memcpy(tem->mKey, key, keyLen);
  return tem;
}


template <typename T>
int Art<T>::CheckPrefixPes(Node<T>* now, const char* key,
                           int keyLen, int depth)
{
  InnerNode<T>* now2 = (InnerNode<T>*)now;
  // 先判断当前节点已存前缀是否匹配
  int mx = std::min(std::min(MAX_PREFIX_LEN, now2->mPrefixLen),
                    keyLen - depth);
  int i;
  for (i = 0; i < mx; i++) {
    if (key[depth+i] != now2->mPrefix[i])
      return i;
  }

  // 若已存前缀完全匹配，需要找到一个叶节点进行匹配
  if (now2->mPrefixLen > MAX_PREFIX_LEN) {
    // TODO: check again
    LeafNode<T>* l = MinLeaf(now);
    mx = std::min(l->mKeyLen, keyLen) - depth;
    for (; i < mx; i++) {
      if (l->mKey[depth+i] != key[depth+i])
        return i;
    }
  }
  
  return i;
}

template <typename T>
LeafNode<T>* Art<T>::MinLeaf(Node<T>* now)
{
  if (now == nullptr) return nullptr;
  if (now->mNodeType == LEAFNODE) return (LeafNode<T>*)now;
  return MinLeaf( MinChild(now) );
}

template <typename T>
InnerNode<T>* Art<T>::Grow(InnerNode<T>* now)
{
  switch (now->mNodeType) {
  case (NODE4): {
    Node4<T>* now1;
    now1 = (Node4<T>*)now;
    Node16<T>* newNode = (Node16<T>*)mNodeAllocator->NewNode(NODE16);
    
    newNode->mPrefixLen = now1->mPrefixLen;
    newNode->mChildrenNum = now1->mChildrenNum;
    memcpy(newNode->mPrefix, now1->mPrefix, MAX_PREFIX_LEN);
    memcpy(newNode->mChildren, now1->mChildren,
           sizeof(Node<T>*) * now1->mChildrenNum);
    memcpy(newNode->mKey, now1->mKey, now1->mChildrenNum);
    return (InnerNode<T>*)newNode;
  }
    
  case (NODE16): {
    Node16<T>* now2;
    now2 = (Node16<T>*)now;
    Node48<T>* newNode2 = (Node48<T>*)mNodeAllocator->NewNode(NODE48);

    newNode2->mPrefixLen = now2->mPrefixLen;
    newNode2->mChildrenNum = now2->mChildrenNum;
    memcpy(newNode2->mPrefix, now2->mPrefix, MAX_PREFIX_LEN);
    memcpy(newNode2->mChildren, now2->mChildren,
           sizeof(Node<T>*) * now2->mChildrenNum);
    for (int i = 0; i < now2->mChildrenNum; i++) {
      newNode2->mIndex[now2->mKey[i] + 128] = i;
    }
    return (InnerNode<T>*)newNode2;
  }
    
  case (NODE48): {
    Node48<T>* now3;
    now3 = (Node48<T>*)now;
    Node256<T>* newNode3 = (Node256<T>*)mNodeAllocator->NewNode(NODE256);

    newNode3->mPrefixLen = now3->mPrefixLen;
    newNode3->mChildrenNum = now3->mChildrenNum;
    memcpy(newNode3->mPrefix, now3->mPrefix, MAX_PREFIX_LEN);
    for (int i = 0; i < 256; i++) {
      if (now3->mIndex[i] != 48) {
        newNode3->mChildren[i] = now3->mChildren[now3->mIndex[i]];
      }
    }
    return (InnerNode<T>*)newNode3;
  }
    
  case (NODE256): {
    throw std::runtime_error("Art::Grow(): Grow NODE256!");
    break;
  }

  default: {
    throw std::runtime_error("Art::Grow(): NodeType error");
  }

  }
}

template <typename T>
Node<T>** Art<T>::FindChild(Node<T>* now, char partialKey)
{
  switch (now->mNodeType) {
  case NODE4: 
    return ((Node4<T>*)now)->FindChild(partialKey);
  case NODE16:
    return ((Node16<T>*)now)->FindChild(partialKey);
  case NODE48:
    return ((Node48<T>*)now)->FindChild(partialKey);
  case NODE256:
    return ((Node256<T>*)now)->FindChild(partialKey);
  default:
    throw std::runtime_error("Art::FindChild() wrong type");
  }
}

template <typename T>
void Art<T>::AddChild(Node<T>* now, char partialKey, Node<T>* child)
{
  switch (now->mNodeType) {
  case NODE4: 
    return ((Node4<T>*)now)->AddChild(partialKey, child);
  case NODE16:
    return ((Node16<T>*)now)->AddChild(partialKey, child);
  case NODE48:
    return ((Node48<T>*)now)->AddChild(partialKey, child);
  case NODE256:
    return ((Node256<T>*)now)->AddChild(partialKey, child);
  default:
    throw std::runtime_error("Art()::AddChild() wrong type");
  }
}

template <typename T>
Node<T>* Art<T>::MinChild(Node<T>* now)
{
  switch (now->mNodeType) {
  case NODE4: 
    return ((Node4<T>*)now)->MinChild();
  case NODE16:
    return ((Node16<T>*)now)->MinChild();
  case NODE48:
    return ((Node48<T>*)now)->MinChild();
  case NODE256:
    return ((Node256<T>*)now)->MinChild();
  default:
    throw std::runtime_error("Art::MinChild() wrong type");
  }
}

template <typename T>
bool Art<T>::IsFull(Node<T>* now)
{
  switch (now->mNodeType) {
  case NODE4: 
    return ((Node4<T>*)now)->IsFull();
  case NODE16:
    return ((Node16<T>*)now)->IsFull();
  case NODE48:
    return ((Node48<T>*)now)->IsFull();
  case NODE256:
    return ((Node256<T>*)now)->IsFull();
  default:
    throw std::runtime_error("Art::IsFull() wrong type");
  }  
}

template <typename T>
Node<T>* Art<T>::CopyNode(Node<T>* now)
{
  Node<T>* newNode = (Node<T>*)mNodeAllocator->NewNode(now->mNodeType);

  switch (now->mNodeType) {
  case NODE4: ((Node4<T>*)now)->CopyNode(newNode); break;
  case NODE16: ((Node16<T>*)now)->CopyNode(newNode); break;
  case NODE48: ((Node48<T>*)now)->CopyNode(newNode); break;
  case NODE256: ((Node256<T>*)now)->CopyNode(newNode); break;
  default:
    throw std::runtime_error("Art::CopyNode() wrong type");
  }

  return newNode;
}

#endif //_Art_H
