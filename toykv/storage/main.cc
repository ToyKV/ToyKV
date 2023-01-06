#include <iostream>
#include "skiplist.h"

#include <fmt/core.h>
#include <memory>
using namespace std;

int main(){

    SkipList<int, IntCmp> skiplist(12);

    for(int i = 1; i <= 10; i++){
        skiplist.Insert(i);
    }

    skiplist.Insert(12);

     for(int i = 1; i <= 10; i++){
        cout << skiplist.Contains(i) << "     ";
    }
    cout << "-----------------" << endl;

    auto iter = SkipList<int, IntCmp>::Iterator(&skiplist);
    for (iter.SeekToFirst(); iter.Valid(); iter.Next()) {
        cout << iter.Key() << endl;

    }
    iter.SeekToLast();
    for(iter.SeekToLast(); iter.Valid(); iter.Prev()) {
        cout << iter.Key() << endl;
    }
    iter.Seek(10);
    cout << iter.Key() << endl;
  
}