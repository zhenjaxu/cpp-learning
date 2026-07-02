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

## 新增功能
### Health组件
新增血量功能，有当前血量和最大血量。
```cpp
struct Health{int hp, maxHp;};
```
除了存放健康值的数组外，还需对实体进行标记，使系统在处理时能够有针对性处理数据，保证逻辑性。
```cpp
std::vector<Health> healths;    // 新增健康值
// 标识实体
std::vector<bool> alive;        // true=存活
std::vector<bool> isBullet;     // true=子弹
std::vector<bool> isEnemy;      // true=敌人
```
比如只有活着的实体才需要更新位置。
```cpp
// update时跳过死亡实体
void update(float dt){
    for(size_t i=0;i<entityCount;++i){
        if(!alive[i]) continue;     // 死亡的实体不更新
        positions[i].x+=velocities[i].vx*dt;
        positions[i].y+=velocities[i].vy*dt;
        colliders[i].x=positions[i].x;
        colliders[i].y=positions[i].y;
    }
}
```
增加这些标识后，每个实体都需要push_back相应的数据（即使不需要），保证数据与id是一一对应的。
```cpp
// 子弹
healths.push_back({1, 1});
alive.push_back(true);
isBullet.push_back(true);
isEnemy.push_back(false);
```
增加扣血功能。
```cpp
// 扣血系统
bool applyDamage(size_t target, int damage){
    if(!alive[target]) return false;
    healths[target].hp-=damage;
    if(healths[target].hp<=0){
        return alive[target]=false;
    }
    return true;
}
```
然后测试一下健康值系统。玩家位于（50，50），两个敌人分别在正下方。子弹从玩家处向正下方射出，会陆续击中，并杀死沿途敌人。击中敌人的子弹会消失。如果是穿透子弹，则不需要对子弹applyDamage。
```cpp
for(int frame=0;frame<30;++frame){
    if(frame%4==0) ecs.spawnBullet(ecs.positions[0].x+10, ecs.positions[0].y);     // 玩家射击
    for(size_t i=0;i<ecs.entityCount;++i){
        if(!ecs.alive[i]) continue;
        if(ecs.isBullet[i]){
            for(size_t j=0;j<ecs.entityCount;++j){
                if(ecs.isEnemy[j]&&ecs.alive[j]&&ecs.checkCollision(i, j)){
                    ecs.applyDamage(i, 50);     // 击中后子弹消失
                    std::cout<<"HIT ENEMY "<<j<<"!\n"
                                <<"ENEMY "<<(ecs.applyDamage(j, 50)?"ALIVE":"DIE!")<<"\n";
                }
            }
        }
    }
    ecs.update(dt);
}
```
```shell
g++ -std=c++11 test_health.cpp -o test_health
.\test_health
```
```shell
HIT ENEMY 1!
ENEMY ALIVE
HIT ENEMY 1!
ENEMY DIE!
HIT ENEMY 2!
ENEMY ALIVE
HIT ENEMY 2!
ENEMY DIE!
```
测试成功！

