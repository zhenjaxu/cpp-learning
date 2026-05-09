# ECS
## ECS是什么
ECS的E，entity（实体）
C，component（组件）
S，system（系统）
它是游戏开发里的主流架构
### 传统OOP（类继承）
这样的写法，一旦需要修改，可能会牵一发动全身
如果子弹不需要速度，或者敌人需要AI组件，继承树会越来越畸形
```cpp
class Player
{
public:
    Position pos;   // 位置
    Velocity vel;   // 速度
    void update()   // 更新位置
    {
        pos.x+=pos.vx;
    }
};

class Enemy : public Player { ... };    // 继承玩家
class Bullet : public Player { ... };   // 子弹也继承玩家
```
### 主流架构ECS
实现数据与逻辑分离
```cpp
// *组件*，纯数据，存在大数组中
ComponentManager cm; 
cm.init(2);     // 确定容量 
// 初始化数据，初始位置和速度
cm.positions[0] = {0, 0}; 
cm.positions[1] = {1, 0.5f};
cm.velocities[0] = {10, 10};
cm.velocities[1] = {5, 0};
// *系统*，处理同一类组件
// 移动系统管理所有物体移动
MovementSystem sys;
for (int frame = 0; frame < 5; ++frame)
{
    // *实体*，就是一个编号，没有任何数据
    std::cout << "frame " << frame << ":\n";
    for (int i = 0; i < 2; ++i)
    {
        std::cout << " Entity " << i << ":("
                  << cm.positions[i].x << ", "
                  << cm.positions[i].y << ")\n";
    }
    // update一次性就能更新所有位置
    sys.update(cm, 2, 1.0f / 60.0f);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}
```
这样做的好处：
1. 想给某个实体加功能，直接往数组里塞数据，用改类定义
2. 系统批量处理数据，CPU缓存友好，数组是连续内存，遍历飞快
3. 逻辑与数据解耦，加新功能很少改动旧代码
## 总结
所以说，传统OOP（类继承）用继承树组织对象，但游戏对象关系本就复杂，后续改动牵一发动全身，继承也容易僵化
而ECS实现数据与逻辑分离，实体（Entity）即编号，组件（Component）纯数据，存在连续数组里，系统（System）批量处理。其扩展性强（加组件）、性能高（缓存友好），是现代游戏引擎的主流架构
