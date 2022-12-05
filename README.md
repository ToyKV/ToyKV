# ToyKV
a kv storage for educational purpose.

The requirements are:
- CMake 3.14+ highly recommended.
- A C++17 compatible compiler (doctest needed!).
- fmt
- doctest (test needed)
- clang-format (optional)
- clang-tidy (optional)
- ccache

## How To Use ?
To configure:
```
cmake -S . -B build
```
Add `-GNinja` if you have Ninja.

if you want to add tests, just add `-DBUILD_TESTS=on`  

To build:
```
cmake --build build
```

To test(`--target` can be written as `-t` in CMake 3.15+):
```
cmake --build build --target test
```

To clang-format:
```
cmake --build build --target clangformat
```

To clang-tidy:
```
cmake --build build --target clangtidy
```

