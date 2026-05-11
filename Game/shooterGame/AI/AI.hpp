#pragma once
#include "../GameECS/GameECS.hpp"
#include <cmath>

enum class AIState{PATROL, CHASE};

struct AIComponent{
    AIState state=AIState::PATROL;
    float patorlRange=100.0f;   // 巡逻范围
    float chaseRange=150.0f;    // 发现玩家距离
    float patrolDir=1.0f;       // 巡逻方向
};

class AISystem{
    int reverse=1;
public:
    void update(GameECS& ecs, size_t enemyIdx, size_t playerIdx, float dt){
        // 假设enemyIdx有AIComponent（需要扩展ECS）
        Position& enemyPos=ecs.positions[enemyIdx];
        Position& playerPos=ecs.positions[playerIdx];

        float dist=std::sqrt(
            std::pow((enemyPos.x-playerPos.x), 2)+
            std::pow((enemyPos.y-playerPos.y), 2)
        );

        Velocity& vel=ecs.velocities[enemyIdx];

        if(dist<100.0f){
            // CHASE：朝玩家移动
            vel.vx=(playerPos.x>enemyPos.x)?2.0f:-2.0f;
            vel.vy=(playerPos.y>enemyPos.y)?2.0f:-2.0f;
        }else{
            // PATROL：左右来回
            if(enemyPos.x>200.0f||enemyPos.x<0) reverse*=-1;
            vel.vx=1.0f*reverse;
            vel.vy=0;
        }
    }
};