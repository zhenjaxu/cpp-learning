# GameECS
## 介绍
将对象池和AABB整合进ECS架构中。
### 实现
对象池可以include之前写的。AABB主要就是比较，可以在成员函数（系统）中实现。
```cpp
#include "../ObjectPool/ObjectPool.hpp"
```
组件纯数据，存在数组里。
```cpp
std::vector<Position> positions;
std::vector<Velocity> velocities;
std::vector<AABB> colliders;
```
实体无数据，就是id。count用于计数，方便遍历所有实体。
```cpp
size_t entityCount=0;
ObjectPool<int> bulletPool{100};    // 用int作id
```
子弹生成系统，暂不考虑释放，所以id为实体计数器值。
```cpp
void spawnBullet(float x, float y){
    int* id=bulletPool.acquire();
    if(!id) return;

    *id=entityCount;
    positions.push_back({x, y});
    velocities.push_back({0, -0.5f});
    colliders.push_back({x, y, 5, 5});
    entityCount++;
}
```
移动系统，一次性处理所有实体的移动。
```cpp
void update(float dt){
    for(size_t i=0;i<entityCount;++i){
        positions[i].x+=velocities[i].vx*dt;
        positions[i].y+=velocities[i].vy*dt;
        colliders[i].x=positions[i].x;
        colliders[i].y=positions[i].y;
    }
}
```
碰撞检测系统，检测两物体是否发生碰撞。
```cpp
void checkCollision(size_t a, size_t b){
    AABB& aa=colliders[a];
    AABB& bb=colliders[b];
    if(aa.x<bb.x+bb.w&&aa.x+aa.w>bb.x&&
        aa.y<bb.y+bb.h&&aa.y+aa.h>bb.h){
        std::cout<<"HIT! "<<a<<" vs "<<b<<"\n";
    }
}
```
### 游戏测试
在终端进行游戏测试，代码和结果如下。
```shell
g++ -std=c++11 main.cpp -o game
./game
```
```shell
HIT! 0 vs 1
```