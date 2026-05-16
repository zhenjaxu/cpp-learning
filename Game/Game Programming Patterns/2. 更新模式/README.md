# 更新模式
## 意图
通过处理一帧的行为模拟一系列独立对象。
## 更新模式
### 存储状态
对于游戏中的AI，例如骷髅守卫（来回移动）。我们需要一个**状态变量**来追踪它的行走方向。当从代码中返回时，就会丢失行走方向。所以为了下一帧继续，需要显式存储足够多的信息。**状态机**就可以做到。
```cpp
Entity skeleton;
bool patrollingLeft=false;      // 状态变量
double x=0;
// 游戏循环
while(true){
    if(patrollingLeft){
        x--;
        if(x==0) patrollingLeft=false;
    }else{
        x++;
        if(x==100) patrollingLeft=true;
    }
    skeleton.setX(x);
    // ...
}
```
### 逐帧模拟
在对象更新列表中，对象之间是有先后顺序的。对于游戏逻辑而言，序列更新可以避免同时更新带来的**竞争**，且对于玩家而言所有对象都是**同时运转**的。
### 修改对象列表
对于更新时增加对象，新对象会增加到列表尾部，这意味着它在当前帧就有机会活动。如果不想发生这种情况，则需在更新前记录对象列表的大小，保证只更新固定数量的对象就停止，不会更新到新增的对象。
```cpp
int numObjectsThisTurn=numObjects_;
for(int i=0;i<numObjectsThisTurn;++i){
    objects_[i]->update();
}
```
麻烦在于移除对象的时候。如果在更新的过程中移除某个对象，而该对象有恰好在当前遍历对象的前面，那列表对象就会整体**前移**，而当前遍历对象的后面一个对象就会被**跳过**（索引向后移动一次，列表对象整体前移一次）。解决方法就是先将对象标记为“死亡”，在更新的时候跳过“死亡”对象，在遍历完列表后再移除。
## 设计决策
### 使用什么类
#### 实体类
每个游戏实体封装自己的行为，在游戏循环update时，在每个对象上调用自己的update，对象就会同时行动。
```cpp
// 实体类
class Entity{
public:
    Entity():x_(0),y_(0){}
    virtual ~Entity(){}
    virtual void update()=0;

    double x() const {return x;}
    double y() const {return y;}

    void setX(double x){x_=x;}
    void setY(double y){y_=y;}

private:
    double x_;
    double y_;
};
```
游戏管理实体的集合，例如游戏世界。游戏通过每帧更新每个实体。
```cpp
// 游戏世界
class World{
public:
    World():numEntities_(0){}
    void gameLoop();            // 游戏循环

private:
    Entity* entities_[MAX_ENTITIES];
    int numEntities_;
};
void World::gameLoop(){
    while(true){
        processInput();
        for(int i=0;i<numEntities;++i){
            entities_[i]->update();     // 每个对象调用直接类的update
        }
        render();
    }
}
```
#### 组件类
游戏业界使用类继承的前辈建立择天蔽日高耸入云的对象层次，最终证明了这是个糟点子，没人可以不拆解它们来管理庞杂的对象层次。最终写道：多用“对象组合”，而非“类继承”。
这个问题的解决方案就是**组件**模式。update更新的是实体的组件，而非实体类。这避开了为了定义和重用行为而创建实体所需的复杂类继承层次。相反，你只需要混合和组装组件。
#### 委托类
可将类的部分行为委托给其他对象。这样做允许你改变委托对象来定义新行为，就像使用组件。
```cpp
void Entity::update(){
    // 转发给状态对象
    state_->update();
}
```
### 隐藏对象
游戏中不是所有对象都需要更新，它们可能停用，可能超出屏幕，或者还没解锁。如果每帧都更新它们就是在浪费CPU循环。
#### 不活跃对象集合
对于不活跃对象，要么检查一些“是否启动”的标识，要么调用一些啥也不做的方法，这会浪费时间。
#### 活跃对象集合
使用额外的内存管理第二个集合。在需要所有实体的时候，这个集合是多余的。在速度比内存要求更高的时候，这种取舍是值得的。
#### 两个集合
权衡之后，选择使用两个集合（活跃与不活跃），这样在需要全部实体的时候，活跃对象集合就不会多余。这种模式需要保持集合**同步**。
