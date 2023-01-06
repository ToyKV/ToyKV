#ifndef STORAGE_NODE_H
#define STORAGE_NODE_H

#include <cstring>

template<typename KeyType, class Comparator>
class Node {
public:
    Node() {}
    Node(KeyType key, int level) {
        this->key = key;
        this->node_level = level;
        this->forward = new Node<KeyType, Comparator> * [level + 1];
        memset(forward, 0, sizeof(Node<KeyType, Comparator>*) * (level + 1));
    }

    ~Node() {
        delete []forward;
    }

    KeyType get_key() {
        return this->key;
    }

    Node<KeyType, Comparator> ** forward; // 存储当前节点在第i层的下一个节点

    int node_level;   // 该节点存在于第[0, node_level]层
private:
    KeyType key;    // key

};


#endif