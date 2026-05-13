#pragma once
#include <vector>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <cmath>

struct Position{float x, y;};
struct Velocity{float vx, vy;};
struct AABB{float x, y, w, h;};
struct Health{int hp, maxhp;};

enum class EntityType{PLAYER, ENEMY, BULLET, NONE};

static const size_t MAX_ENTITIES=100;
static const size_t INVALID_ID = static_cast<size_t>(-1);

std::unordered_map<EntityType, std::vector<float>> initMap={
    // vx(0), vy(1), w(2), h(3), hp(4), maxhp(5)
    {EntityType::PLAYER, {0, 0, 30.0f, 30.0f, 100.0f, 100.0f}},
    {EntityType::ENEMY, {5.0f, -5.0f, 30.0f, 30.0f, 50.0f, 50.0f}},
    {EntityType::BULLET, {0, 5.0f, 5.0f, 5.0f, 1.0f, 1.0f}},
    {EntityType::NONE, {0, 0, 0, 0, 0, 0}}
};

class ECS{
    // 内存块
    std::vector<Position> positions;
    std::vector<Velocity> velocities;
    std::vector<AABB> colliders;
    std::vector<Health> healths;
    std::vector<bool> alive;
    std::vector<EntityType> types;      // 实体标签

    std::queue<size_t> freeSlots;       // 回收死亡的槽位
    size_t slotCount=0;                 // 已经push_back过的总槽位数
    const float dt;                      // 帧间隔时间
    int reverse=1;                      // 控制敌机转向
    const float chaseRange=50.0f;     // 敌机返现玩家距离
    const int bulletWound=25;
    const int enemyWound=25;
    const int playerWound=50;
    
    // 申请内存
    size_t acquire(EntityType type, float x, float y){
        size_t id;
        if(!freeSlots.empty()){
            id=freeSlots.front();
            freeSlots.pop();
            positions[id]={x, y};
            velocities[id]={initMap[type][0], initMap[type][1]};
            colliders[id]={x, y, initMap[type][2], initMap[type][3]};
            alive[id]=true;
            healths[id]={(int)initMap[type][4], (int)initMap[type][5]};
            types[id]=type;
        }else if(slotCount<MAX_ENTITIES){
            id=slotCount++;
            positions.push_back({x, y});
            velocities.push_back({initMap[type][0], initMap[type][1]});
            colliders.push_back({x, y, initMap[type][2], initMap[type][3]});
            alive.push_back(true);
            healths.push_back({(int)initMap[type][4], (int)initMap[type][5]});
            types.push_back(type);
        }else{
            std::cout<<"Pool full!\n";
            return INVALID_ID;
        }
        return id;
    }
    // 释放
    void release(size_t id){
        if(id>=slotCount) return;
        alive[id]=false;
        types[id]=EntityType::NONE;
        freeSlots.push(id);
    }
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
    // 检查碰撞
    bool checkCollision(size_t a, size_t b){
        AABB& ac=colliders[a];
        AABB& bc=colliders[b];
        return ac.x<bc.x+bc.w&&ac.x+ac.w>bc.x&&
               ac.y<bc.y+bc.h&&ac.y+ac.h>bc.y;
    }
    // 造成伤害
    void applyDamage(size_t targetId, int damage){
        if(!alive[targetId]) return;
        if((healths[targetId].hp-=damage)<=0){
            alive[targetId]=false;
        }
    }

public:
    ECS(float dt_):dt(dt_){
        // 内存块预分配
        positions.reserve(MAX_ENTITIES);
        velocities.reserve(MAX_ENTITIES);
        colliders.reserve(MAX_ENTITIES);
        healths.reserve(MAX_ENTITIES);
        alive.reserve(MAX_ENTITIES);
        types.reserve(MAX_ENTITIES);
        // 固定生成玩家0（35，0）
        acquire(EntityType::PLAYER, 35.0f, 0);
    }
    // 生成敌人
    size_t spawnEnemy(float enemyX, float enemyY){
        return acquire(EntityType::ENEMY, enemyX, enemyY);
    }
    // 生成子弹
    size_t spawnBullet(size_t id){
        if(alive[id]&&types[id]==EntityType::PLAYER) 
            return acquire(EntityType::BULLET, 47.5f, 0);
        return INVALID_ID;
    }
    // 更新位置
    void update(){
        for(size_t id=0;id<slotCount;++id){
            if(!alive[id]) continue;
            if(types[id]==EntityType::ENEMY){
                if(positions[id].y<0) release(id);
                updateAI(id, 0);
            }
            if(types[id]==EntityType::BULLET){
                if(positions[id].y>150) release(id);
            }
            positions[id].x+=velocities[id].vx*dt;
            positions[id].y+=velocities[id].vy*dt;
            colliders[id].x=positions[id].x;
            colliders[id].y=positions[id].y;
        }
    }
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
};