#ifndef NODE_H
#define NODE_H

#include <string.h>


template<typename K, typename V>
class Node {
public:
    //构造函数
    Node();

    Node(K k, V v, int level) {
        this->key = k;
        this->value = v;
        this->node_level = level;
        this->forward = new Node<K,V>*[level + 1];
        memset(forward, 0, sizeof(Node<K,V>*) * (level + 1));
    }

    ~Node(){
        delete[] forward;
    }

    K get_key() const {         //得到当前节点的key
       return this->key; 
    }
 
    V get_value() const {       // 得到当前节点的value
        return this->value;
    }
    
    void set_value(V v)  {     // 设置value值
        this->value = v;
    }
    
    Node<K, V> **forward;       // 存储当前节点在i层的下一个节点

    int node_level;         // 该节点存在于[0, node_level]层

private:
    K key;
    V value;
};



#endif