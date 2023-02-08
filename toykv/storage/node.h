#pragma once

#include <cstring>

template <typename KeyType, class Comparator>
class Node {
 public:
  Node() = default;
  Node(KeyType key, int level) {
    this->key_ = key;
    this->node_level_ = level;
    this->forward_ = new Node<KeyType, Comparator> *[level + 1];
    memset(forward_, 0, sizeof(Node<KeyType, Comparator> *) * (level + 1));
  }

  ~Node() { delete[] forward_; }

  KeyType GetKey() { return this->key_; }

  Node<KeyType, Comparator> **forward_;  // 存储当前节点在第i层的下一个节点

  int node_level_;  // 该节点存在于第[0, node_level]层
 private:
  KeyType key_;  // key
};
