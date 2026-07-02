#pragma once
#include "../ObjectPool/ObjectPool.hpp"
#include <vector>
#include <iostream>

struct Position{float x, y;};
struct Velocity{float vx, vy;};
struct AABB{float x, y, w, h;};
struct Health{int hp, maxHp;};      // 新增健康值

class GameECS{
public:
    std::vector<Position> positions;
    std::vector<Velocity> velocities;
    std::vector<AABB> colliders;
    std::vector<Health> healths;    // 新增健康值
    // 标识实体
    std::vector<bool> alive;        // true=存活
    std::vector<bool> isBullet;     // true=子弹
    std::vector<bool> isEnemy;      // true=敌人
    size_t entityCount=0;

    ObjectPool<int> bulletPool{100};    // 用int作id
    
    void spawnBullet(float x, float y){
        int* id=bulletPool.acquire();
        if(!id) return;

        *id=entityCount;
        positions.push_back({x, y});
        velocities.push_back({0, -5.0f});
        colliders.push_back({x, y, 5, 5});
        healths.push_back({1, 1});
        alive.push_back(true);
        isBullet.push_back(true);
        isEnemy.push_back(false);
        entityCount++;
    }

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

    bool checkCollision(size_t a, size_t b){
        AABB& aa=colliders[a];
        AABB& bb=colliders[b];
        if(aa.x<bb.x+bb.w&&aa.x+aa.w>bb.x&&
           aa.y<bb.y+bb.h&&aa.y+aa.h>bb.h){
            return true;
        }
        return false;
    }

    // 生成敌人（有血量）
    void spawnEnemy(float x, float y){
        positions.push_back({x, y});
        velocities.push_back({0, 0});
        colliders.push_back({x, y, 20, 20});
        healths.push_back({100, 100});
        alive.push_back(true);
        isBullet.push_back(false);
        isEnemy.push_back(true);
        entityCount++;
    }

    // 扣血系统
    bool applyDamage(size_t target, int damage){
        if(!alive[target]) return false;
        healths[target].hp-=damage;
        if(healths[target].hp<=0){
            return alive[target]=false;
        }
        return true;
    }
};