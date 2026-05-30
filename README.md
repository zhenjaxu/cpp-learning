# Cpp-Learning

> C++ 系统学习笔记与代码实践。涵盖现代C++特性（STL、智能指针、并发）、LeetCode算法题解以及游戏开发学习。

## 📁 仓库结构

| 目录 | 内容说明 |
|------|---------|
| [`Concurrency/`](./Concurrency) | C++11/14/17 并发编程：线程、锁、条件变量、基于锁的并发数据结构 |
| [`Game/`](./Game) | 游戏开发学习：游戏模式、SDL2 图形库、游戏循环与游戏对象、CMake 构建 |
| [`Leetcode/`](./Leetcode) | LeetCode Hot 100 题解，包含思路注释与复杂度分析 |
| [`ThreadPool/`](./ThreadPool) | 手写线程池实现，练习现代C++并发与模板编程 |

## 🔨 技术栈

- **语言**: C++14
- **构建**: CMake, Makefile, MinGW
- **图形**: SDL2
- **版本控制**: Git
- **环境**: Windows + Ubuntu 虚拟机

## 🚀 构建与运行

```bash
# 克隆仓库
git clone https://github.com/Don-qq/Cpp-learning.git
cd Cpp-learning

# 以 Pong 为例
cd Game\Programming\Game\Pong
cmake -G "MinGW Makefiles" -B build
cmake --build build
.\build\game