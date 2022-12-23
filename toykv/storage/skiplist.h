#ifndef SKIPLIST_H
#define SKIPLIST_H

#include "node.h"
#include <math.h>
#include <string>

template<typename K, typename V>
class SkipList {
public:

    SkipList(int max_level);

    ~SkipList();

    bool insert_element(K k, V v);      // 插入，成功true，失败false

    bool update_element(K k, V v);      // 更新数据

    bool delete_element(K k);       // 删除，成功true，失败false

    V query_element(K k);       // 查找，成功返回value；失败返回NULL

    void show_all();           // 展示数据

    int size();     // 元素的个数
private:

    int get_random_level();         // 获得一个随机的层数

    Node<K, V>* create_node(K k, V v, int level);    // 创建节点

private:
    Node<K, V>* m_head; // 头

    int m_count; // 节点个数

    int m_max_level; // 跳表的最大的层数

    int m_current_level;    // 跳表目前的层数

};



#endif