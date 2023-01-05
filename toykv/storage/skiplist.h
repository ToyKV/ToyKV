#pragma once

struct IntCmp {
  bool operator()(int a, int b) { return a < b; }
};

// TODO(pgj): 更换IntCmp，这个模板参数应该必须让用户传入
// 我这里给的IntCmp只是为了方便测试
template <typename KeyType, class Comparator = IntCmp>
class SkipList {
 public:
  void Insert(const KeyType &key);

  bool Contains(const KeyType &key);

  class Iterator {
   public:
    explicit Iterator(const SkipList &list);

    const KeyType Key() const ;
    bool Valid();
    void Next();
    void Prev();
    void Seek(const KeyType &key);
    void SeekToFirst();
    void SeekToLast();
  };
};
