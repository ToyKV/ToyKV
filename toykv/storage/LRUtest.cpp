#include <bits/stdc++.h>
#include "LRU.h"
// using namespace std;
#define capacity 3
// #define Ktype int //Key类型
// #define Vtype int //Value类型
#define Ktype std::string  // Key类型
#define Vtype int          // Value类型
int main() {
  //   Vtype a;
  //  auto test=new LRUCache<Ktype,Vtype>(capacity);
  //  test->put(1,1);
  //   a= test->get(1); cout<<a<<endl;

  //  test->put(2,1);
  //   a= test->get(2); cout<<a<<endl;

  //  test->put(2,2);
  //  a= test->get(2); cout<<a<<endl;

  //  test->put(3,3);
  //  test->put(4,4);
  //  a= test->get(1); cout<<a<<endl;

  Vtype a;
  auto test = new LRUCache<Ktype, Vtype>(capacity);
  test->put("tom", 3);
  a = test->get("tom");
  std::cout << a << std::endl;

  test->put("jack", 4);
  a = test->get("jack");
  std::cout << a << std::endl;

  a = test->get("Alice");
  std::cout << a << std::endl;

  return 0;
}
