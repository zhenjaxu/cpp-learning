#include "ECS.hpp"
#include <iostream>
#include <random>

int main(){
    const float frameRate=60.0f;      // 游戏帧数
    const float enemySpawn=6.0f;     // 敌人生成时间间隔
    const float bulletSpawn=3.0f;    // 子弹生成时间间隔
    const float runtime=1000.0f;       // 游戏运行时间
    // 左右边界（敌人生成范围）
    const float leftBoundary=0;
    const float rightBoundary=100;
    // 敌人生成高度（y坐标）
    const float spawnEnemyHighth=150;

    ECS ecs(1.0f/frameRate);    // 创建游戏并生成玩家0
    bool gameOver=false;
    size_t frame=0;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(leftBoundary, rightBoundary);

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
}