#pragma once
#ifndef STORAGE_SKIPLIST_H
#define STORAGE_SKIPLIST_H

#include <time.h>
#include <cstdlib>
#include <iostream>
#include "node.h"
struct IntCmp {
  int operator()(int a, const int &b) const { return a - b; }
};

// TODO(pgj): 更换IntCmp，这个模板参数应该必须让用户传入
// 我这里给的IntCmp只是为了方便测试
template <typename KeyType, class Comparator>
class SkipList {
 public:
  // 插入
  // 要求：当前的列表中没有和key一样的节点
  void Insert(const KeyType &key);

  // 返回是否有和key相等的节点
  bool Contains(const KeyType &key);

  explicit SkipList(int);

  ~SkipList();

  class Iterator {
   public:
    explicit Iterator(const SkipList *list);

    // 如果迭代器位于有效节点，返回true
    bool Valid();

    const KeyType Key() const;

    void Next();

    // 前进到上一个位置
    // 要求： valid()
    void Prev();

    // 到达大于等于target的第一个位置
    void Seek(const KeyType &key);

    // 达到列表中的第一个位置
    // 迭代器的最终状态时valid()并且该列表不空
    void SeekToFirst();

    // 到达列表的最后一个位置
    // 迭代器的最终状态时valid()并且该列表不空
    void SeekToLast();

   private:
    Node<KeyType, Comparator> *m_node;

    const SkipList *m_list;
  };

 private:
  Node<KeyType, Comparator> *m_head;

  Comparator m_compare;

  // 拥有的节点数
  int m_count;

  // 跳表的最大层数
  int m_max_level;

  // 跳表的目前的层数
  int m_currnet_level;

 private:
  // 获得一个随机的层数
  int get_random_level();

  // 创建新节点
  Node<KeyType, Comparator> *create_node(KeyType key, int level);

  // 找到第一位 < key的节点
  Node<KeyType, Comparator> *find_less_than(const KeyType &key) const;

  // 找到第一位 >= key的节点
  Node<KeyType, Comparator> *find_greater_or_equal(const KeyType &key) const;

  // 找到最后一位节点
  Node<KeyType, Comparator> *find_last() const;
};

template <typename KeyType, class Comparator>
SkipList<KeyType, Comparator>::SkipList(int level)
    : m_compare(Comparator()), m_count(0), m_max_level(level), m_currnet_level(0) {
  KeyType key;
  m_head = new Node<KeyType, Comparator>(key, m_max_level);
}

template <typename KeyType, class Comparator>
SkipList<KeyType, Comparator>::~SkipList() {
  delete m_head;
}

template <typename KeyType, class Comparator>
Node<KeyType, Comparator> *SkipList<KeyType, Comparator>::create_node(KeyType key, int level) {
  return new Node<KeyType, Comparator>(key, level);
}

template <typename KeyType, class Comparator>
int SkipList<KeyType, Comparator>::get_random_level() {
  int level = 0;
  unsigned int seed = time(NULL);
  for (;; level++) {
    if (rand_r(&seed) % 2 == 0) {
      return level < m_max_level ? level : m_max_level;
    }
  }
}

template <typename KeyType, class Comparator>
void SkipList<KeyType, Comparator>::Insert(const KeyType &key) {
  // curr 用于遍历跳表
  Node<KeyType, Comparator> *curr = this->m_head;
  // update用于保存要插入的位置的上一位
  Node<KeyType, Comparator> *update[m_max_level + 1];
  memset(update, 0, sizeof(Node<KeyType, Comparator> *) * (m_max_level + 1));

  // 从跳表的最高层开始，查找相当于“二分查找”
  for (int i = m_currnet_level; i >= 0; i--) {
    while (curr->forward[i] != NULL && m_compare(curr->forward[i]->get_key(), key) < 0) {
      curr = curr->forward[i];
    }
    update[i] = curr;
  }

  curr = curr->forward[0];

  if (curr == NULL || curr->get_key() != key) {
    // 每一层插入不插入概率50%，得到一个随机层数
    int random_level = get_random_level();

    // 如果random_level > m_current_level，超出的部分要添加头
    if (random_level > m_currnet_level) {
      for (int i = m_currnet_level + 1; i < random_level + 1; i++) {
        update[i] = m_head;
      }
      // 更新m_current_level
      m_currnet_level = random_level;
    }

    // 生成要插入的节点
    Node<KeyType, Comparator> *target_node = create_node(key, random_level);

    // 插入节点
    for (int i = 0; i <= random_level; i++) {
      target_node->forward[i] = update[i]->forward[i];
      update[i]->forward[i] = target_node;
    }
    m_count++;
  }
}

template <typename KeyType, class Comparator>
bool SkipList<KeyType, Comparator>::Contains(const KeyType &key) {
  // curr 用于遍历
  Node<KeyType, Comparator> *curr = m_head;

  // 从跳表的最高层开始"二分查找"
  for (int i = m_currnet_level; i >= 0; i--) {
    while (curr->forward[i] != NULL && (m_compare(curr->forward[i]->get_key(), key) < 0)) {
      curr = curr->forward[i];
    }
  }
  curr = curr->forward[0];

  if (curr != NULL && m_compare(curr->get_key(), key) == 0) {
    return true;
  }
  return false;
}

template <typename KeyType, class Comparator>
Node<KeyType, Comparator> *SkipList<KeyType, Comparator>::find_less_than(const KeyType &key) const {
  // 没有节点
  if (m_count == 0) return m_head;
  Node<KeyType, Comparator> *curr = m_head;
  for (int i = m_currnet_level; i >= 0; i--) {
    while (curr->forward[i] != NULL && m_compare(curr->forward[i]->get_key(), key) < 0) {
      curr = curr->forward[i];
    }
  }
  return curr;
}

template <typename KeyType, class Comparator>
Node<KeyType, Comparator> *SkipList<KeyType, Comparator>::find_greater_or_equal(const KeyType &key) const {
  if (m_count == 0) return m_head;

  Node<KeyType, Comparator> *curr = m_head;

  for (int i = m_currnet_level; i >= 0; i--) {
    while (curr->forward[i] != NULL && m_compare(curr->forward[i]->get_key(), key) < 0) {
      curr = curr->forward[i];
    }
  }
  curr = curr->forward[0];
  // 如果存在这个数，就返回这个数的位置
  return curr;
}

template <typename KeyType, class Comparator>
Node<KeyType, Comparator> *SkipList<KeyType, Comparator>::find_last() const {
  Node<KeyType, Comparator> *curr = m_head;
  for (int i = m_currnet_level; i >= 0; i--) {
    while (curr->forward[i] != NULL) curr = curr->forward[i];
  }
  return curr;
}

template <typename KeyType, class Comparator>
inline SkipList<KeyType, Comparator>::Iterator::Iterator(const SkipList *list) {
  m_list = list;
  m_node = NULL;
}

template <typename KeyType, class Comparator>
inline bool SkipList<KeyType, Comparator>::Iterator::Valid() {
  return m_node != nullptr;
}

template <typename KeyType, class Comparator>
inline const KeyType SkipList<KeyType, Comparator>::Iterator::Key() const {
  return m_node->get_key();
}

template <typename KeyType, class Comparator>
inline void SkipList<KeyType, Comparator>::Iterator::Next() {
  m_node = m_node->forward[0];
}

template <typename KeyType, class Comparator>
inline void SkipList<KeyType, Comparator>::Iterator::Prev() {
  m_node = m_list->find_less_than(m_node->get_key());
  if (m_node == m_list->m_head) m_node = NULL;
}

template <typename KeyType, class Comparator>
inline void SkipList<KeyType, Comparator>::Iterator::Seek(const KeyType &key) {
  m_node = m_list->find_greater_or_equal(key);
}

template <typename KeyType, class Comparator>
inline void SkipList<KeyType, Comparator>::Iterator::SeekToFirst() {
  m_node = m_list->m_head->forward[0];
}

template <typename KeyType, class Comparator>
inline void SkipList<KeyType, Comparator>::Iterator::SeekToLast() {
  m_node = m_list->find_last();
}

#endif
