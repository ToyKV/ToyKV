#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <fmt/core.h>
#include <memory>
#include "toykv/storage/skiplist.h"

TEST_CASE("test skiplist") {
  auto skip_list = std::make_unique<SkipList<int, IntCmp>>(12);
  for (int i = 0; i < 10; ++i) {
    skip_list->Insert(i);
  }
  fmt::print("--------\n");

  auto iter = std::make_unique<SkipList<int, IntCmp>::Iterator>(skip_list.get());
  for (iter->SeekToFirst(); iter->Valid(); iter->Next()) {
    fmt::print("{}\n", iter->Key());
  }
  fmt::print("--------\n");

  for (iter->SeekToLast(); iter->Valid(); iter->Prev()) {
    fmt::print("{}\n", iter->Key());
  }
}

TEST_CASE("skiplist benchmark") {
  // TODO(pgj): add benchmark
}
