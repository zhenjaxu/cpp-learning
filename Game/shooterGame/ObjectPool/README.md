# 对象池
## 介绍
对象池，用于存放各种对象的池子。
### 实现
基于模板类型推导，实现各种对象统一管理。pool预分配内存，available存放指向可用内存的指针。
```cpp
std::vector<T> pool;        // 预分配的内存块
std::queue<T*> available;   // 空闲对象队列
```
reserve是预分配内存，不构造/不销毁，与resize不同，避免频繁重新分配，对性能敏感场景（如循环内大量push_back）适用。
```cpp
ObjectPool(size_t size){
        pool.reserve(size);
        for(size_t i=0;i<size;++i){
            pool.emplace_back();            // 原地构造，不new
            available.push(&pool.back());   // 加入空闲队列
        }
    }
```
成员函数主要有两个，一个申请内存，一个释放内存。只对available进行操作，实际内存还在，只是指针（权限）的转移。这样可以极大地避免耗时的构造和析构。
```cpp
T* acquire(){
    if(available.empty()){
        std::cout<<"Pool empty! Connect spawn.\n";
        return nullptr;
    }
    T* obj=available.front();
    available.pop();
    return obj;     // 复用已有内存
}
```
```cpp
void release(T* obj){
    available.push(obj);    // 回收，不delete
}
```
### 测试案例
用对象池管理子弹。子弹有编号和坐标。
```cpp
struct Bullet{
    int id=0;
    float x=0, y=0;
};
```
将结构体传入模板即可获得针对该对象的统一管理，内存分配从一开始就完成了。后面3个对象的申请只是操作指针，开销极小。
```cpp
ObjectPool<Bullet> pool(3);

Bullet* b1=pool.acquire(); b1->id=1; b1->x=100;
Bullet* b2=pool.acquire(); b2->id=2; b2->x=200;
Bullet* b3=pool.acquire(); b3->id=3; b3->x=300;
```
当没有空闲内存时，会返回提示信息。释放掉b2后，available回收内存，然后交给b5。此时，b5要么初始化，要么覆盖掉b2的数据。
```cpp
// 第四个应该失败，超出容量
Bullet* b4=pool.acquire();  // 预期输出：Pool empty！

// 回收b2，在申请应该复用同一块内存
pool.release(b2);
Bullet* b5=pool.acquire();  // 复用了b2的内存
std::cout<<"Reused bullet at address: "<<b5<<", old was: "<<b2<<"\n";
std::cout<<"b5 id: "<<b5->id<<"(should be 2, unless you reset it)\n";
```

## 总结
对象池通过模板推导，可以接受各种类型的对象（结构体），通过预分配和指针，实现快速内存分配和内存管理，提高游戏性能。