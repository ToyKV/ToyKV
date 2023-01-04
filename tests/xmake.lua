add_requires("doctest", {system = false})

target("test_hello")
  set_kind("binary")
  set_group("tests")
  add_files("$(projectdir)/tests/test_hello.cpp")
  add_packages("doctest")

target("test_skiplist")
  set_kind("binary")
  set_group("tests")
  add_files("$(projectdir)/tests/storage/test_skiplist.cpp")
  add_packages("doctest", "fmt")
  add_deps("toykv")
