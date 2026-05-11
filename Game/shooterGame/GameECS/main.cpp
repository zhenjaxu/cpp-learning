#include "GameECS.hpp"

int main(){
    GameECS ecs;

    // 玩家（entity 0）
    ecs.positions.push_back({50, 50});
    ecs.velocities.push_back({0, 0});
    ecs.colliders.push_back({50, 50, 20, 20});
    ecs.entityCount++;

    // 子弹（entity 1）。在玩家附近，应该碰撞
    ecs.spawnBullet(55, 55);

    // 子弹（entity 2）。很远，不碰撞
    ecs.spawnBullet(200, 200);

    // 检测玩家（0）与其他所有实体
    for(size_t i=1;i<ecs.entityCount;++i){
        ecs.checkCollision(0, i);
    }

    return 0;
}