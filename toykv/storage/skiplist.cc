#include"skiplist.h"
#include <iostream>
template<typename K, typename V>
SkipList<K, V>::SkipList(int max_level) : m_max_level(max_level), 
                    m_current_level(0), m_count(0){
    K k;
    V v;
    this->m_head = new Node<K, V>(k, v, max_level);
}


template<typename K, typename V>
SkipList<K, V>::~SkipList() {
    delete m_head;
}


template<typename K, typename V>
Node <K, V> * SkipList<K, V>::create_node(K k, V v, int level) {
    return new Node<K, V>(k, v, level);
}


template<typename K, typename V>
int SkipList<K, V>::size() {
    return m_count;
}

template<typename K, typename V>
int SkipList<K, V>::get_random_level() {
   
   // 生成随机层数，一个节点在[0, random_level]层
   int level = 0;
   for(; ; level++) {
        // 成功的概率是50%，在0层的概率是1，在1层的概率是50%，下一层是25%，依次类推
        if(rand() % 2 == 0) {
            return level < m_max_level ? level : m_max_level;
        }
   }
}

template<typename K, typename V>
bool SkipList<K, V>::insert_element(K k, V v) {

    Node<K, V>* curr = this->m_head;        
    Node<K, V>* update[m_max_level + 1];        //  update数组表示要插入的位置的上一位
    memset(update, 0, sizeof(Node<K,V>*) * (m_max_level + 1));

    // 从跳表的最高层开始
    for(int i = m_current_level; i >= 0; i--) {
        while(curr->forward[i] != NULL && curr->forward[i]->get_key() < k) {
            curr = curr->forward[i];
        }
        update[i] = curr;
    }

    curr = curr->forward[0];

    // 如果当前的key等于要插入的k，返回false，插入失败
    if(curr != NULL && curr->get_key() == k) {
        return false;
    }

    // 找到每层的update之后，curr会向后退一位
    if(curr == NULL || curr->get_key() != k) {

        // 生成一个随机层数random_level，在0和m_max_level之间
        int random_level = get_random_level();

        // 如果random_level > m_current_level，超出的部分要添加头
        if(random_level > m_current_level) {
            for(int i = m_current_level + 1; i < random_level + 1; i++) {
                update[i] = m_head;
            }
            //更新当前跳表的层数
            m_current_level = random_level;
        }

        Node<K, V>* target_node = create_node(k, v, random_level);

        // 插入节点
        for(int i = 0; i <= random_level; i++) {
            target_node->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = target_node;
        }
        m_count ++;
    }
    // 插入成功
    return true;

}

template<typename K, typename V>
bool SkipList<K, V>::update_element(K k, V v) {

    Node<K, V> *curr = m_head;

    // 与插入类似，从跳表的最高层开始
    for(int i = m_current_level; i >= 0; i--) {
        while(curr->forward[i] != NULL && curr->forward[i]->get_key() < k) {
            curr = curr->forward[i];
        }
    }

    curr = curr->forward[0];

    //修改成功，返回true
    if(curr != NULL && curr->get_key() == k) {
        curr->set_value(v);
        return true;
    }

    // 修改失败，返回false
    return false;
}


template<typename K, typename V>
V SkipList<K, V>::query_element(K k) {

    Node<K, V> *curr = m_head;

    // 与插入类似，从跳表的最高层开始
    for(int i = m_current_level; i >= 0; i--) {
        while(curr->forward[i] != NULL && curr->forward[i]->get_key() < k) {
            curr = curr->forward[i];
        }
    }

    curr = curr->forward[0];

    // 查询成功，返回value
    if(curr != NULL && curr->get_key() == k) {
        return curr->get_value();
    }

    //查询失败，返回NULL
    return NULL;
}

template<typename K, typename V>
bool SkipList<K, V>::delete_element(K k) {

    Node<K, V> *curr = this->m_head;
    Node<K, V> *update[m_max_level + 1];        // update用于保存待删除的节点的上一个节点
    memset(update, 0, sizeof(Node<K,V>*) * (m_max_level + 1));

    // 从跳表的最高层开始,和插入类似
    for(int i = m_current_level; i >= 0; i--) {
        while(curr->forward[i] != NULL && curr->forward[i]->get_key() < k) {
            curr = curr->forward[i];
        }
        update[i] = curr;
    }

    // curr向后一位curr指的就是待删除的节点
    curr = curr->forward[0];

    // k 不存在，返回false
    if(curr == NULL || curr->get_key() != k) {
        return false;
    }
    
    // curr指向删除的节点
    if(curr != NULL && curr->get_key() == k) {

        for(int i = 0; i <= m_current_level; i++) {
            if(update[i]->forward[i] != curr)  break;
            
            // 删除节点
            update[i]->forward[i] = curr->forward[i];
        }
    }

    // 适当降低高度
    while(m_current_level > 0 && m_head->forward[m_current_level] == 0) {
        m_current_level--;
    }

    //删除成功
    return true;
}

template<typename K, typename V>
void SkipList<K, V>::show_all() {
    
    for(int i = 0; i <= m_current_level; i++) {
        Node<K, V> *tmp = this->m_head->forward[i];
        std::cout << "Level " << i << ":" << std::endl;
        while(tmp != NULL) {
            std::cout << tmp->get_key() << ":" << tmp->get_value() << ";" <<std::endl;
            tmp = tmp->forward[i];
        }
        std::cout << std::endl;
    }
}

/*
int main () {

    SkipList<int, int> skiplist(32);

    for(int i = 1; i <= 20; i++) {
        skiplist.insert_element(i, i*10);
    }
    skiplist.show_all();
    std::cout << "*---------------------------------------*" << std::endl;
    std::cout << "the value of key 1 : " << skiplist.query_element(1) << std::endl;
    std::cout << "the value of key 2 : " << skiplist.query_element(2) << std::endl;
    std::cout << "the value of key 3 : " << skiplist.query_element(3) << std::endl;
    std::cout << "the value of key 4 : " << skiplist.query_element(4) << std::endl;
    skiplist.delete_element(5);
    skiplist.update_element(7, 90);
    std::cout << skiplist.update_element(200, 90) <<std::endl;
    skiplist.show_all();
    return 0;
}
*/
