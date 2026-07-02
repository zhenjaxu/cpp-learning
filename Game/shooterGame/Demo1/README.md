# 射击游戏（案例1）
## 介绍
一款简易版雷霆战机射击游戏。
## 内部模块
整合当前对象池、AABB碰撞、AI系统、Health组件以及ECS架构。
### 对象池
对象池作为实体内存分配的关键，其先开辟空间，后管理指针的方式，使运行速度和游戏性能得到了很大的提升。
本案例利用其思路，先为各个组件（数据数组）开辟空间，再通过管理编号的方式，实现内存申请（acquire）和逻辑上资源释放（release）。
```cpp
struct Position{float x, y;};
struct Velocity{float vx, vy;};
struct AABB{float x, y, w, h;};
struct Health{int hp, maxhp;};

enum class EntityType{PLAYER, ENEMY, BULLET, NONE};
```
```cpp
// 内存块
std::vector<Position> positions;
std::vector<Velocity> velocities;
std::vector<AABB> colliders;
std::vector<Health> healths;
std::vector<bool> alive;
std::vector<EntityType> types;      // 实体标签
```
以上都是存放数据的数组，包括位置、速度、边框、血量、存活以及标签。标签用于标识实体类型，以便初始化，也方便系统对数据的针对处理。
```cpp
std::unordered_map<EntityType, std::vector<float>> initMap={
    // vx(0), vy(1), w(2), h(3), hp(4), maxhp(5)
    {EntityType::PLAYER, {0, 0, 30.0f, 30.0f, 100.0f, 100.0f}},
    {EntityType::ENEMY, {5.0f, -5.0f, 30.0f, 30.0f, 50.0f, 50.0f}},
    {EntityType::BULLET, {0, 5.0f, 5.0f, 5.0f, 1.0f, 1.0f}},
    {EntityType::NONE, {0, 0, 0, 0, 0, 0}}
};
```
下面是ECS的构造，通过resize直接开辟空间。本案例只有一个玩家，在构造的时候生成。dt为帧时间间隔，作为参数赋值给成员变量。MAX_ENTITIES为最大实体数量，限制同时存在游戏中的实体数量。
```cpp
static const size_t MAX_ENTITIES=100;
```
```cpp
ECS(float dt_):dt(dt_){
    // 内存块预分配
    positions.resize(MAX_ENTITIES);
    velocities.resize(MAX_ENTITIES);
    colliders.resize(MAX_ENTITIES);
    healths.resize(MAX_ENTITIES);
    alive.resize(MAX_ENTITIES, false);
    types.resize(MAX_ENTITIES, EntityType::NONE);
    // 固定生成玩家0（35，0）
    acquire(EntityType::PLAYER, 35.0f, 0);
}
```
对象池还有资源的申请和释放。通过freeSlots实现编号的回收，通过slotCount记录后续初始化的位置。当slotCount等于最大实体数量时，且freeSlots里面没有空闲编号，就表示对象池已满，返回一个无效编号（-1）。
```cpp
static const size_t INVALID_ID = static_cast<size_t>(-1);
```
```cpp
std::queue<size_t> freeSlots;       // 回收死亡的槽位
size_t slotCount=0;                 // 已经初始化过的总槽位数
```
```cpp
// 申请内存
size_t acquire(EntityType type, float x, float y){
    size_t id;
    if(!freeSlots.empty()){
        id=freeSlots.front();freeSlots.pop();
    }
    else if(slotCount<MAX_ENTITIES) id=slotCount++;   
    else{
        std::cout<<"Pool full!\n";
        return INVALID_ID;
    }
    // 覆盖原来是数据
    positions[id]={x, y};
    velocities[id]={initMap[type][0], initMap[type][1]};
    colliders[id]={x, y, initMap[type][2], initMap[type][3]};
    alive[id]=true;
    healths[id]={(int)initMap[type][4], (int)initMap[type][5]};
    types[id]=type;

    return id;
}
```
释放的关键在于数据赋值和编号回收。回收前需将alive和types修改，防止系统对已回收数据进行处理。
```cpp
// 释放
void release(size_t id){
    if(id>=slotCount) return;
    alive[id]=false;
    types[id]=EntityType::NONE;
    freeSlots.push(id);
}
```
### AABB碰撞
AABB碰撞就是通过边框重合条件判定两个实体是否发生碰撞。如果碰撞返回true，反之返回false。
```cpp
bool checkCollision(size_t a, size_t b){
    AABB& ac=colliders[a];
    AABB& bc=colliders[b];
    return ac.x<bc.x+bc.w&&ac.x+ac.w>bc.x&&
           ac.y<bc.y+bc.h&&ac.y+ac.h>bc.y;
}
```
### Health血量
当前案例的血量模块最主要就是扣血系统。扣除血量并判断实体是否死亡。
```cpp
// 造成伤害
void applyDamage(size_t targetId, int damage){
    if(!alive[targetId]) return;
    if((healths[targetId].hp-=damage)<=0){
        alive[targetId]=false;
    }
}
```
### AI模块
AI模块关键在于更新敌机的状态，改变其速度和方向。使其朝玩家移动，对玩家造成威胁。通过reverse*=-1，实现敌机来回移动（越界就转向）。
```cpp
int reverse=1;                      // 控制敌机转向
const float chaseRange=50.0f;     // 敌机发现玩家距离
```
```cpp
// 更新敌机状态
void updateAI(size_t enemyId, size_t playerId){
    Position& enemyPos=positions[enemyId];
    Position& playerPos=positions[playerId];
    float dist=std::sqrt(
        std::pow((enemyPos.x-playerPos.x), 2)+
        std::pow((enemyPos.y-playerPos.y), 2)
    );
    Velocity& vel=velocities[enemyId];
    if(dist<chaseRange){
        // CHASE：朝玩家移动
        if(playerPos.x>enemyPos.x) vel.vx=5.0f;
        else if(playerPos.x<enemyPos.x) vel.vx=-5.0f;
        else vel.vx=0;
    }else{
        // PATROL：左右来回
        if(enemyPos.x>100.0f||enemyPos.x<0) reverse*=-1;
        vel.vx=5.0f*reverse;
    }
}
```
## 外部接口
### 位置更新系统
游戏的每一帧都会进行位置更新。首先是规避掉已经死亡的实体。回收超出边界的实体。而且在位置更新之前，还需要更新敌机状态。
```cpp
// 游戏上下边界
const int upperBoundary=150;
const int lowerBoundary=0;
```
```cpp
// 更新位置
void update(){
    for(size_t id=0;id<slotCount;++id){
        if(!alive[id]) continue;
        // 回收越界实体
        if(types[id]==EntityType::ENEMY){
            if(positions[id].y<lowerBoundary) release(id);
            updateAI(id, 0);    // 顺便更新敌机状态
        }
        if(types[id]==EntityType::BULLET){
            if(positions[id].y>upperBoundary) release(id);
        }
        positions[id].x+=velocities[id].vx*dt;
        positions[id].y+=velocities[id].vy*dt;
        colliders[id].x=positions[id].x;
        colliders[id].y=positions[id].y;
    }
}
```
### 生成实体系统
本案例玩家只有一个，无需提供玩家生成的接口。敌人生成需要位置信息，子弹生成则需要发射子弹的实体id（暂不支持敌人）。
```cpp
// 生成敌人
void spawnEnemy(float enemyX, float enemyY){
    acquire(EntityType::ENEMY, enemyX, enemyY);
}
// 生成子弹
void spawnBullet(size_t id){
    if(alive[id]&&types[id]==EntityType::PLAYER) 
        acquire(EntityType::BULLET, 47.5f, 0);
}
```
### 实体碰撞系统
借助AABB碰撞、扣血等内部模块，实现不同类型实体间相互碰撞的外部接口。
子弹攻击敌人时，子弹消失（扣血），敌人减少生命值。释放死亡实体资源。
```cpp
// 检查子弹是否攻击到敌人
void checkBulletEnemyCollisions(){
    for(size_t bulletId=0;bulletId<slotCount;++bulletId){
        if(!alive[bulletId]||types[bulletId]!=EntityType::BULLET) continue;
        for(size_t enemyId=0;enemyId<slotCount;++enemyId){
            if(!alive[enemyId]||types[enemyId]!=EntityType::ENEMY) continue;
            if(checkCollision(bulletId, enemyId)){
                applyDamage(bulletId, enemyWound);
                applyDamage(enemyId, bulletWound);
                bool enemyAlive=alive[enemyId];
                std::cout<<"Bullet "<<bulletId<<" hit enemy "<<enemyId<<(enemyAlive?"\n":"(DIE)\n");
                if(!enemyAlive) release(enemyId);
                if(!alive[bulletId]) release(bulletId);
            }
        } 
    }
}
```
敌人会追击并撞击玩家造成伤害，最后判断玩家是否活着并返回。
```cpp
// 检查是敌人是否撞到玩家
bool checkEnemyPlayerCollisions(){
    for(size_t enemyId=0;enemyId<slotCount;++enemyId){
        if(!alive[enemyId]||types[enemyId]!=EntityType::ENEMY) continue;
        if(checkCollision(enemyId, 0)){
            applyDamage(enemyId, playerWound);
            applyDamage(0, enemyWound);
            std::cout<<"Enemy "<<enemyId<<" hit player"<<(alive[0]?"\n":"(DIE)\n");
            if(!alive[enemyId]) release(enemyId);
            return alive[0];
        }
    }
    return true;
}
```
## 游戏测试
### 测试案例
设置游戏参数并初始化游戏。伪随机数生成
```cpp
const float frameRate=60.0f;      // 游戏帧数
const float enemySpawn=6.0f;     // 敌人生成时间间隔
const float bulletSpawn=3.0f;    // 子弹生成时间间隔
const float runtime=1000.0f;       // 游戏运行时间
// 左右边界（敌人生成范围）
const float leftBoundary=0;
const float rightBoundary=100.0f;
// 敌人生成高度（y坐标）
const float spawnEnemyHighth=150;

ECS ecs(1.0f/frameRate);    // 创建游戏并生成玩家0
bool gameOver=false;
size_t frame=0;
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> dis(leftBoundary, rightBoundary);
```
开始游戏，直到时间结束或玩家阵亡。
```cpp
while(!gameOver&&frame!=(int)(runtime*frameRate)){
    if(frame%(int)(enemySpawn*frameRate)==0) ecs.spawnEnemy(dis(gen), 150);
    if(frame%(int)(bulletSpawn*frameRate)==0) ecs.spawnBullet(0);
        
    ecs.checkBulletEnemyCollisions();
    gameOver=!ecs.checkEnemyPlayerCollisions();
    ecs.update();
    frame++;
}
std::cout<<(gameOver?"GAME OVER!\n":"TIME OVER!");
std::cout<<"Game time: "<<frame/frameRate<<"\n";
return 0;
```
### 运行代码
在终端上编译运行。348秒的游戏内容在不到一秒的时间内全部输出，游戏性能不错。
```shell
g++ -std=c++11 main.cpp -o game
./game
```
```shell
Bullet 2 hit enemy 4
Bullet 3 hit enemy 4(DIE)
Bullet 5 hit enemy 7
Bullet 8 hit enemy 1
Bullet 9 hit enemy 1(DIE)
...
Bullet 14 hit enemy 3(DIE)
Enemy 8 hit player
Enemy 1 hit player(DIE)
GAME OVER!
Game time: 348.317
```
游戏运行正常，频繁生成实体也并没有导致对象池爆满，资源释放和内存复用正常。