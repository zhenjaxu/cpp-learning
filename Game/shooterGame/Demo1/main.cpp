#include "DemoECS.hpp"
#include <iostream>
#include <random>

int main(){
    float frameRate=60.0f;      // 游戏帧数
    ECS ecs(1.0f/frameRate);    // 设置30帧并生成玩家0（45，0）
    bool gameOver=false;
    size_t frame=0;

    float enemySpawn=6.0f;     // 敌人生成时间间隔
    float bulletSpawn=3.0f;    // 子弹生成时间间隔
    float runtime=1000.0f;       // 游戏运行时间

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 100.0f);

    while(!gameOver&&frame!=(int)(runtime*frameRate)){
        if(frame%(int)(enemySpawn*frameRate)==0) ecs.spawnEnemy(dis(gen), 150);
        if(frame%(int)(bulletSpawn*frameRate)==0) ecs.spawnBullet(0);
            
        ecs.checkBulletEnemyCollisions();
        gameOver=!ecs.checkEnemyPlayerCollisions();
        ecs.update();
        frame++;
    }
    std::cout<<(gameOver?"GAME OVER!\n":"TIME OVER!");
    return 0;
}