#include "GameECS.hpp"

int main(){
    GameECS ecs;
    // 玩家0
    ecs.positions.push_back({50, 50});
    ecs.velocities.push_back({0, 0});
    ecs.colliders.push_back({50, 50, 20, 20});
    ecs.healths.push_back({100, 100});
    ecs.alive.push_back(true);
    ecs.isEnemy.push_back(false);
    ecs.isBullet.push_back(false);
    ecs.entityCount++;
    // 敌人1
    ecs.spawnEnemy(50, 25);
    // 敌人2
    ecs.spawnEnemy(50, 0);

    float dt=0.8f;

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
}