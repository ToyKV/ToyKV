#include <bits/stdc++.h>
//using namespace std;
// 双向链表节点定义，泛型key与value
template <class K, class V>
class Node {
 public:
  K key;
  V value;
  Node<K, V> *pre;
  Node<K, V> *next;
  // 构造函数初始化
  Node(K key, V value) : key(key), value(value), pre(nullptr), next(nullptr) {}
};
// LRU缓存定义，可调用get,put方法
template <class K, class V>
class LRUCache {
 private:
  int size;          // 缓冲区大小
  Node<K, V> *head;  // 靠近头部的节点为最近使用
  Node<K, V> *tail;  // 缓冲区满时淘汰尾部的节点
  map<K, Node<K, V> *> p;

 public:
  LRUCache(int capacity) {
    this->size = capacity;
    head = nullptr;
    tail = nullptr;
  }
  // 获取缓冲区中 key 对应的 value
  V get(K key) {
    // 1.当该 key 值存在
    if (p.count(key) > 0) {
      // 删除该 key 对应的原来节点
      Node<K, V> *cur = p[key];
      int value = cur->value;
      remove(cur);  // 这里仅仅删除哈希双向链表中的节点，不必删除哈希表中的
      // 将节点重现插入到缓冲区的头部
      setHead(cur);
      return value;
    }
    // 2.当该 key 值不存在
    return -1;
  }
  // 将key-value值存入缓冲区
  void put(K key, V value) {
    // 1.当该 key 值存在
    if (p.count(key) > 0) {
      // 删除该 key 对应的原来节点
      Node<K, V> *cur = p[key];
      cur->value = value;
      remove(cur);  // 这里仅仅删除哈希双向链表中的节点，不必删除哈希表中的
      // 将节点重新插入到缓冲区的头部
      setHead(cur);
    } 
    else { // 2.当该 key 值不存在 
      Node<K, V> *node = new Node<K, V>(key, value);
      // 判断当前缓冲区大小已经满了
      if (p.size() >= size) {
        // 删除尾部节点
        // map<int, Node<K,V> *>::iterator it = p.find(tail->key);// 返回迭代器类型
        remove(tail);
        // p.erase(V);// 这里erase 函数参数是迭代器类型，所以上面需要使用迭代器类型
        // 将新节点插入到缓冲区的头部
      }
      // else 此时因为动作和上面重复，所以直接合并使用
      // 还没有满：将新节点插入到缓冲区的头部
      {
        setHead(node);
        p[key] = node;
      }
    }
  }

  // 删除当前节点
  void remove(Node<K, V> *cur) {
    // 当前节点是 head
    if (cur == head)
      head = cur->next;
    else if (cur == tail)  // 当前节点是 tail
      tail = cur->pre;
    else   // 当前节点是一般节点
      cur->pre->next = cur->next, cur->next->pre = cur->pre;
  }
  // 将当前节点插入到头部
  void setHead(Node<K, V> *cur) {
    cur->next = head;
    if (head != nullptr) head->pre = cur;
    head = cur;  // 重现更新head

    if (tail == nullptr) tail = head;
  }
};
