#pragma once
#include "../ObjectPool/ObjectPool.hpp"
#include <vector>
#include <iostream>

struct Position{float x, y;};
struct Velocity{float vx, vy;};
struct AABB{float x, y, w, h;};

class GameECS{
public:
    std::vector<Position> positions;
    std::vector<Velocity> velocities;
    std::vector<AABB> colliders;
    size_t entityCount=0;

    ObjectPool<int> bulletPool{100};    // 用int作id
    
    void spawnBullet(float x, float y){
        int* id=bulletPool.acquire();
        if(!id) return;

        *id=entityCount;
        positions.push_back({x, y});
        velocities.push_back({0, -0.5f});
        colliders.push_back({x, y, 5, 5});
        entityCount++;
    }

    void update(float dt){
        for(size_t i=0;i<entityCount;++i){
            positions[i].x+=velocities[i].vx*dt;
            positions[i].y+=velocities[i].vy*dt;
            colliders[i].x=positions[i].x;
            colliders[i].y=positions[i].y;
        }
    }

    void checkCollision(size_t a, size_t b){
        AABB& aa=colliders[a];
        AABB& bb=colliders[b];
        if(aa.x<bb.x+bb.w&&aa.x+aa.w>bb.x&&
           aa.y<bb.y+bb.h&&aa.y+aa.h>bb.h){
            std::cout<<"HIT! "<<a<<" vs "<<b<<"\n";
        }
    }
};