#include <bits/stdc++.h>
using namespace std;
//跳表的最大高度
constexpr int MAX_LEVEL = 32;

struct SkiplistNode {
  int val;
  vector<SkiplistNode *> forward;
  SkiplistNode(int _val, int _maxLevel = MAX_LEVEL)
      : val(_val), forward(_maxLevel, nullptr) {}
};

class Skiplist {
 private:
  SkiplistNode *head;
  int level;
  uniform_real_distribution<double> dis;

 public:
  Skiplist() : head(new SkiplistNode(-1)), level(0), dis(0, 1) {}

  bool search(int target) {
    SkiplistNode *curr = this->head;
    for (int i = level - 1; i >= 0; i--) {
      /* 找到第 i 层小于且最接近 target 的元素*/
      while (curr->forward[i] && curr->forward[i]->val < target) {
        curr = curr->forward[i];
      }
    }
    curr = curr->forward[0];
    /* 检测当前元素的值是否等于 target */
    if (curr && curr->val == target) {
      return true;
    }
    return false;
  }

  void add(int num) {
    vector<SkiplistNode *> update(MAX_LEVEL, head);
    SkiplistNode *curr = this->head;
    for (int i = level - 1; i >= 0; i--) {
      /* 找到第 i 层小于且最接近 num 的元素*/
      while (curr->forward[i] && curr->forward[i]->val < num) {
        curr = curr->forward[i];
      }
      update[i] = curr;
    }
    int lv = randomLevel();
    level = max(level, lv);
    SkiplistNode *newNode = new SkiplistNode(num, lv);
    for (int i = 0; i < lv; i++) {
      /* 对第 i 层的状态进行更新，将当前元素的 forward 指向新的节点 */
      newNode->forward[i] = update[i]->forward[i];
      update[i]->forward[i] = newNode;
    }
  }

  bool erase(int num) {
    vector<SkiplistNode *> update(MAX_LEVEL, nullptr);
    SkiplistNode *curr = this->head;
    for (int i = level - 1; i >= 0; i--) {
      /* 找到第 i 层小于且最接近 num 的元素*/
      while (curr->forward[i] && curr->forward[i]->val < num) {
        curr = curr->forward[i];
      }
      update[i] = curr;
    }
    curr = curr->forward[0];
    /* 如果值不存在则返回 false */
    if (!curr || curr->val != num) {
      return false;
    }
    for (int i = 0; i < level; i++) {
      if (update[i]->forward[i] != curr) {
        break;
      }
      /* 对第 i 层的状态进行更新，将 forward 指向被删除节点的下一跳 */
      update[i]->forward[i] = curr->forward[i];
    }
    delete curr;
    /* 更新当前的 level */
    while (level > 1 && head->forward[level - 1] == nullptr) {
      level--;
    }
    return true;
  }

  int randomLevel() {
    int lv = 1;
    /* 随机生成 lv，概率因子为0.5 */
    while (rand() % 2 == 1) {
      lv++;
    }
    return lv;
  }
};
int main() {
  srand(time(0));
  Skiplist *obj = new Skiplist();
  obj->add(1);
  obj->add(2);
  obj->add(4);
  obj->add(5);
  bool a = obj->search(6);
  cout << a << endl;
  a = obj->search(5);
  cout << a << endl;
  bool b = obj->erase(1);
  a = obj->search(1);
  cout << a << endl;
  system("pause");
  return 0;
}
