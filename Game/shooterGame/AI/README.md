# AI
## 介绍
欲给敌机赋予AI行为，PATROL（巡逻），CHASE（追击）
### 实现
用枚举类保存两种状态。
```cpp
enum class AIState{PATROL, CHASE};
```
需要一个组件和一个系统。先简单实现下基本功能，后加入ECS里面。
```cpp
struct AIComponent{
    AIState state=AIState::PATROL;
    float patorlRange=100.0f;   // 巡逻范围
    float chaseRange=150.0f;    // 发现玩家距离
    float patrolDir=1.0f;       // 巡逻方向
};
```
update用于根据与玩家的相隔距离更新敌机状态。先不考虑组件影响，实现简单功能。
```cpp
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
            if(enemyPos.x>200.0f||enemyPos.x<0) reverse*=-1;    // 在[0, 200]范围内来回巡逻
            vel.vx=1.0f*reverse;
            vel.vy=0;
        }
    }
};
```
### 测试
然后测试一下update功能是否正常。有两个玩家，首先是在范围外的玩家0，敌机会一直巡逻。直到玩家1出现在范围内，敌机立马调转方向追击，最后在玩家附近来回移动。为了更快的呈现，将dt（帧间隔时间）调整至10。
```shell
g++ -std=c++11 test_ai.cpp -o test_ai
./test_ai 
```
```shell
frame 19, player 0
 enemy pos: (200, 0)
frame 20, player 0
 enemy pos: (210, 0)
frame 21, player 0
 enemy pos: (200, 0)
frame 22, player 0
 enemy pos: (190, 0)
```
```shell
frame 0, player 1
 enemy pos: (110, 0)
frame 1, player 1
 enemy pos: (100, 0)
frame 2, player 1
 enemy pos: (90, 0)
frame 3, player 1
 enemy pos: (70, 20)
frame 4, player 1
 enemy pos: (50, 0)
frame 5, player 1
 enemy pos: (30, 20)
frame 6, player 1
 enemy pos: (10, 0)
frame 7, player 1
 enemy pos: (-10, 20)
```

## 总结
通过ai系统通过玩家位置，更新敌机状态，运动方向和速度。而所有物体（包括敌机）的移动依然通过GameECS里面的移动系统（ecs.update()）完成。