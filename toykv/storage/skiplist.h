#pragma once

#include <time.h>
#include <cstdlib>
#include <iostream>
#include "toykv/storage/node.h"
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
    Node<KeyType, Comparator> *node_;

    const SkipList *list_;
  };

 private:
  Node<KeyType, Comparator> *head_;

  Comparator compare_;

  // 拥有的节点数
  int count_;

  // 跳表的最大层数
  int max_level_;

  // 跳表的目前的层数
  int currnet_level_;

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
    : compare_(Comparator()), count_(0), max_level_(level), currnet_level_(0) {
  KeyType key;
  head_ = new Node<KeyType, Comparator>(key, max_level_);
}

template <typename KeyType, class Comparator>
SkipList<KeyType, Comparator>::~SkipList() {
  delete head_;
}

template <typename KeyType, class Comparator>
Node<KeyType, Comparator> *SkipList<KeyType, Comparator>::create_node(KeyType key, int level) {
  return new Node<KeyType, Comparator>(key, level);
}

template <typename KeyType, class Comparator>
int SkipList<KeyType, Comparator>::get_random_level() {
  int level = 0;
  unsigned int seed = time(nullptr);
  for (;; level++) {
    if (rand_r(&seed) % 2 == 0) {
      return level < max_level_ ? level : max_level_;
    }
  }
}

template <typename KeyType, class Comparator>
void SkipList<KeyType, Comparator>::Insert(const KeyType &key) {
  // curr 用于遍历跳表
  Node<KeyType, Comparator> *curr = this->head_;
  // update用于保存要插入的位置的上一位
  Node<KeyType, Comparator> *update[max_level_ + 1];
  memset(update, 0, sizeof(Node<KeyType, Comparator> *) * (max_level_ + 1));

  // 从跳表的最高层开始，查找相当于“二分查找”
  for (int i = currnet_level_; i >= 0; i--) {
    while (curr->forward_[i] != nullptr && compare_(curr->forward_[i]->get_key(), key) < 0) {
      curr = curr->forward_[i];
    }
    update[i] = curr;
  }

  curr = curr->forward_[0];

  if (curr == nullptr || curr->get_key() != key) {
    // 每一层插入不插入概率50%，得到一个随机层数
    int random_level = get_random_level();

    // 如果random_level > m_current_level，超出的部分要添加头
    if (random_level > currnet_level_) {
      for (int i = currnet_level_ + 1; i < random_level + 1; i++) {
        update[i] = head_;
      }
      // 更新m_current_level
      currnet_level_ = random_level;
    }

    // 生成要插入的节点
    Node<KeyType, Comparator> *target_node = create_node(key, random_level);

    // 插入节点
    for (int i = 0; i <= random_level; i++) {
      target_node->forward_[i] = update[i]->forward_[i];
      update[i]->forward_[i] = target_node;
    }
    count_++;
  }
}

template <typename KeyType, class Comparator>
bool SkipList<KeyType, Comparator>::Contains(const KeyType &key) {
  // curr 用于遍历
  Node<KeyType, Comparator> *curr = head_;

  // 从跳表的最高层开始"二分查找"
  for (int i = currnet_level_; i >= 0; i--) {
    while (curr->forward_[i] != nullptr && (compare_(curr->forward_[i]->get_key(), key) < 0)) {
      curr = curr->forward_[i];
    }
  }
  curr = curr->forward_[0];

  if (curr != nullptr && compare_(curr->get_key(), key) == 0) {
    return true;
  }
  return false;
}

template <typename KeyType, class Comparator>
Node<KeyType, Comparator> *SkipList<KeyType, Comparator>::find_less_than(const KeyType &key) const {
  // 没有节点
  if (count_ == 0) return head_;
  Node<KeyType, Comparator> *curr = head_;
  for (int i = currnet_level_; i >= 0; i--) {
    while (curr->forward_[i] != nullptr && compare_(curr->forward_[i]->get_key(), key) < 0) {
      curr = curr->forward_[i];
    }
  }
  return curr;
}

template <typename KeyType, class Comparator>
Node<KeyType, Comparator> *SkipList<KeyType, Comparator>::find_greater_or_equal(const KeyType &key) const {
  if (count_ == 0) return head_;

  Node<KeyType, Comparator> *curr = head_;

  for (int i = currnet_level_; i >= 0; i--) {
    while (curr->forward_[i] != nullptr && compare_(curr->forward_[i]->get_key(), key) < 0) {
      curr = curr->forward_[i];
    }
  }
  curr = curr->forward_[0];
  // 如果存在这个数，就返回这个数的位置
  return curr;
}

template <typename KeyType, class Comparator>
Node<KeyType, Comparator> *SkipList<KeyType, Comparator>::find_last() const {
  Node<KeyType, Comparator> *curr = head_;
  for (int i = currnet_level_; i >= 0; i--) {
    while (curr->forward_[i] != nullptr) curr = curr->forward_[i];
  }
  return curr;
}

template <typename KeyType, class Comparator>
inline SkipList<KeyType, Comparator>::Iterator::Iterator(const SkipList *list) {
  list_ = list;
  node_ = nullptr;
}

template <typename KeyType, class Comparator>
inline bool SkipList<KeyType, Comparator>::Iterator::Valid() {
  return node_ != nullptr;
}

template <typename KeyType, class Comparator>
inline const KeyType SkipList<KeyType, Comparator>::Iterator::Key() const {
  return node_->get_key();
}

template <typename KeyType, class Comparator>
inline void SkipList<KeyType, Comparator>::Iterator::Next() {
  node_ = node_->forward_[0];
}

template <typename KeyType, class Comparator>
inline void SkipList<KeyType, Comparator>::Iterator::Prev() {
  node_ = list_->find_less_than(node_->get_key());
  if (node_ == list_->head_) node_ = nullptr;
}

template <typename KeyType, class Comparator>
inline void SkipList<KeyType, Comparator>::Iterator::Seek(const KeyType &key) {
  node_ = list_->find_greater_or_equal(key);
}

template <typename KeyType, class Comparator>
inline void SkipList<KeyType, Comparator>::Iterator::SeekToFirst() {
  node_ = list_->head_->forward_[0];
}

template <typename KeyType, class Comparator>
inline void SkipList<KeyType, Comparator>::Iterator::SeekToLast() {
  node_ = list_->find_last();
}
