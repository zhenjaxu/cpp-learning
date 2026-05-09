#pragma once
#include <vector>
#include <queue>
#include <iostream>

template<typename T>
class ObjectPool{
    std::vector<T> pool;        // 预分配的内存块
    std::queue<T*> available;   // 空闲对象队列

public:
    ObjectPool(size_t size){
        pool.reserve(size);
        for(size_t i=0;i<size;++i){
            pool.emplace_back();            // 原地构造，不new
            available.push(&pool.back());   // 加入空闲队列
        }
    }

    T* acquire(){
        if(available.empty()){
            std::cout<<"Pool empty! Connect spawn.\n";
            return nullptr;
        }
        T* obj=available.front();
        available.pop();
        return obj;     // 复用已有内存
    }

    void release(T* obj){
        available.push(obj);    // 回收，不delete
    }
};