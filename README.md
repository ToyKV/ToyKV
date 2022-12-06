# ToyKV
a kv storage for educational purpose.

The requirements are:
- CMake 3.14+ highly recommended.
- A C++17 compatible compiler (doctest needed!).
- fmt
- doctest (test needed)
- clang-format 
- clang-tidy 
- cpplint
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

## 应该遵守的规范

### 代码规范
我们使用`Google Style`作为代码规范，所有代码提交前需检查代码是否符合规范：
- 使用`make clangforamt`进行代码格式化
- 使用`make check-lint`检查代码是否符合`Google Style`
- 使用`make check-clang-tidy`再次检查代码
以上提到的内容依赖下面3种工具:
- clang-format
- clang-tidy
- cpplint

### git commit 规范
commit message格式:
```
<type>(<scope>): <subject>
type(必须)
```
用于说明git commit的类别，只允许使用下面的标识。
- feat：新功能（feature）。
- fix/to：修复bug，可以是QA发现的BUG，也可以是研发自己发现的BUG。
  - fix：产生diff并自动修复此问题。适合于一次提交直接修复问题
  - to：只产生diff不自动修复此问题。适合于多次提交。最终修复问题提交时使用fix
- docs：文档（documentation）。
- style：格式（不影响代码运行的变动）。
- refactor：重构（即不是新增功能，也不是修改bug的代码变动）。
- perf：优化相关，比如提升性能、体验。
- test：增加测试。
- chore：构建过程或辅助工具的变动。
- revert：回滚到上一个版本。
- merge：代码合并。
- sync：同步主线或分支的Bug。

scope(可选)
scope用于说明 commit 影响的范围，比如数据层、控制层、视图层等等，视项目不同而不同。
例如在Angular，可以是location，browser，compile，compile，rootScope， ngHref，ngClick，ngView等。如果你的修改影响了不止一个scope，你可以使用*代替。

subject(必须)
subject是commit目的的简短描述，不超过50个字符。

结尾不加句号或其他标点符号。
根据以上规范git commit message将是如下的格式：
```
fix(DAO):用户查询缺少username属性 
feat(Controller):用户查询接口开发
```
以上git规范来自于：https://zhuanlan.zhihu.com/p/182553920
