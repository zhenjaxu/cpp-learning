#include "AI.hpp"

int main(){
    AISystem enemy;
    GameECS ecs;
    float dt=10.0f;

    // 玩家0静止在（100，100）处
    ecs.positions.push_back({100, 100});
    ecs.velocities.push_back({0, 0});
    ecs.colliders.push_back({100, 100, 20, 20});
    ecs.entityCount++;

    // 玩家1静止在（0，20）处
    ecs.positions.push_back({0, 20});
    ecs.velocities.push_back({0, 0});
    ecs.colliders.push_back({0, 20, 20, 20});
    ecs.entityCount++;

    // 敌机1在（0，0）处，应当追击玩家2，而对于玩家1则巡逻
    ecs.positions.push_back({0, 0});
    ecs.velocities.push_back({0, 0});
    ecs.colliders.push_back({0, 0, 20, 20});
    ecs.entityCount++;
    for(size_t id=0;id<2;++id){
        for(size_t frame=0;frame<30;++frame){
            enemy.update(ecs, 2, id, dt);
            ecs.update(dt);
            std::cout<<"frame "<<frame
                     <<", player "<<id
                     <<"\n enemy pos: ("
                     <<ecs.positions[2].x<<", "
                     <<ecs.positions[2].y<<")\n";         
        }
    }
    return 0;
}