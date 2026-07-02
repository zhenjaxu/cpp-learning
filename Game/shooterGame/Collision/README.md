# AABB碰撞
## 介绍
AABB（Axis-Aligned Bounding Box），与坐标轴对齐的边界框。通过比较两个物体的上下左右边界，来判断是否有重叠部分，是否相交（intersect）。
### 实现
AABB通过左下角坐标加上高度和宽度，确定自身位置和实体范围。当两个AABB发生重叠时，重叠处形成新的矩形，必然同时满足下面四个条件。
```cpp
struct AABB{
    float x, y;
    float w, h;

    // AABB碰撞检测：两个矩形是否重叠
    bool intersects(AABB const& other) const{
        return x<other.x+other.w&&      // 左边缘在对方右边缘的左边
               x+w>other.x&&            // 右边缘在对方左边缘的右边
               y<other.y+other.h&&      // 下边缘在对方上边缘的下边
               y+h>other.y;             // 上边缘在对方下边缘的上边
    }
};
```
### 碰撞测试
玩家如果与敌机碰撞，就发动攻击，反之则躲避。
```cpp
AABB player={0,0,50,50};        // 玩家：50x50
AABB enemy1={30,30,50,50};      // 敌机1；重叠，应该HIT
AABB enemy2={100,100,50,50};    // 敌机2：不重叠，应该MISS

std::cout<<"Test 1 (should HIT): "
            <<(player.intersects(enemy1)?"HIT!":"MISS")<<"\n";
std::cout<<"Test 2 (should MISS): "
            <<(player.intersects(enemy2)?"HIT!":"MISS")<<"\n";
```
在终端上运行，玩家向敌机1发动攻击，而没有攻击敌机2。
```shell
g++ -std=c++11 test_collision.cpp -o test_collision
./test_collision
```
```shell
Test 1 (should HIT): HIT!
Test 2 (should MISS): MISS
```

## 总结
与坐标轴对其的边界框（AABB）赋予实体边界，巧妙地判断是否重叠碰撞，以进行下一步决策。