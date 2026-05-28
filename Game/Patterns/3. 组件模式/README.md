# 组件模式
## 目的
允许单一的实体跨越多个领域，而不会导致这些领域彼此解耦。
## 问题
### 代码冗杂
如果将AI、物理、渲染、声音等领域代码全部放在一个类当中，其代码规模难以想象。为类添加**错误**可能比添加**功能**的速度快得多。
### 耦合
比起单纯的规模问题，更严重的是耦合。修改代码难度高，必须保证修改部分不会影响到其他功能。
## 解决方案
按领域将类分割成相互独立的部分，再用一个类将所有的组件包含，各组件实现解耦。
在实际运作中，各组件需要一些**相互作用**。例如，AI组件可能需要告诉物理组件实体的方向。
但需要将这些交互限制在**确实**需要发生交互的组件之间。
但这种解决方法会带来组件间**沟通的复杂**（内存的使用），以及**糟糕的性能**（多一层跳转）。
## 模式
组件通常在定义游戏的核心部分使用。这个模式应该应用在如下情况：
- 有一个涉及了多个领域的类，需要保持这些领域的隔离；
- 一个类正在变而且越来越难以使用；
- 想要能定义一系列分享不同能力的类，但使用类继承无法精准选取到重用的部分。
### 单块类
先展示单块类，**没有**使用组件。
```cpp
class Bjorn{
public:
    Bjorn():velocity_(0),x_(0),y_(0){}
    void update(World& world, Graphics& graphics);

private:
    static const int WALK_ACCELERATION=1;       // 行走加速度

    int velocity_;
    int x_, y_;

    Volume volume_;         // 体积
    // 图像
    Sprite spriteStand_;
    Sprite spriteWalkLeft_;
    Sprite spriteWalkRight_;
};
```
其每帧调用的update方法。
```cpp
void Bjorn::update(World& world, Graphics& graphics){
    // 根据用户输入修改英雄的速度
    switch(Controller::getJoystickDirection()){
        case DIR_LEFT:
            velocity_-=WALK_ACCELERATION;
            break;
        case DIR_RIGHT:
            velocity_+=WALK_ACCELERATION;
            break;
    }
    // 根据速度修改位置
    x_+=velocity_;
    world.resolveCollision(volume_, x_, y_, velocity_);
    // 绘制合适的图形
    Sprite* sprite=&spriteStand_;
    if(velocity_<0){
        sprite=&spriteWalkLeft_;
    }else if(velocity_>0){
        sprite=&spriteWalkRight_;
    }
    graghics.draw(*sprite, x_, y_);
}
```
### 组件类
将单块类按领域分离，首先是处理输入的组件。
```cpp
class InputComponent{
public:
    void update(Bjorn& bjorn){
        switch(Controller::getJoystickDirection()){
            case DIR_LEFT:
                bjorn.velocity-=WALK_ACCELERATION;
                break;
            case DIR_RIGHT:
                bjorn.velocity+=WALK_ACCELERATION;
                break;
        }
    }
private:
    static const int WALK_ACCELERATION=1;
};
```
然后是物理组件，和渲染组件。
```cpp
class PhysicsComponent{
public:
    void update(Bjorn& bjorn, World& world){
        bjorn.x+=bjorn.velocity;
        world.resolveCollision(volume_,
            bjorn.x, bjorn.y, bjorn.veloctiy);
    }

private:
    Volume volume_;
};
```
```cpp
class GraphicsComponent{
public:
    void update(Bjorn& bjorn, Graphics& graphics){
        Sprite* sprite=&spriteStand_;
        if(bjorn.velocity<0){
            sprite=&spriteWalkLeft_;
        }else if(bjorn.velocity>0){
            sprite=&spriteWalkRight_;
        }
        graphics.draw(*sprite, bjorn.x, bjorn.y);
    }

private:
    Sprite spriteStand_;
    Sprite spriteWalkLeft_;
    Sprite spriteWalkRight_;
};
```
最后全部组合进Bjorn类里。速度和位置是“泛领域”状态，几乎所有组件都**需要**。所以将它们提出来，作为组件间沟通的**桥梁**。
```cpp
class Bjorn{
public:
    int x, y;
    int velocity;

    Bjorn():velocity(0),x(0),y(0){}
    void update(){
        input_.update(*this);
        physics_.update(*this, world);
        graphics_.update(*this, graphics);
    }
private:
    InputComponent intput_;
    PhysicsComponet physics_;
    GraphicsComponent graphics_;
};
```
### 组件抽象化
当前组件类都是具体的。比如输入组件有位移、攻击等，需要什么用什么。这就需要借助抽象基类。
```cpp
class InputComponent{
public:
    virtual ~InputComponent(){}
    virtual void update(Bjorn& bjorn)=0;
};
```
```cpp
class PlayerInputComponent:public InputComponent{
public:
    virtual void update(Bjorn& bjorn){
        switch(Controller::getJoystickDirection()){
            case DIR_LEFT:
                bjorn.velocity-=WALK_ACCELERATION;
                break;
            case DIR_RIGHT:
                bjorn.velocity+=WALK_ACCELERATION;
                break;
        }
    }

private:
    static const int WALK_ACCELERATION=1;
};
```
这样Bjorn就只会拥有一个指向输入的指针，而不是一个内联的实例。
```cpp
class Bjorn{
public:
    int velocity;
    int x, y;

    Bjorn(InputComponent* input):input_(input){}
    void update(World& world, Graphics& graphics){
        input_->update(*this);
        physics_.update(*this, world);
        graphics_.update(*this, graphics);
    }

private:
    InputComponent* input_;
    PhysicsComponent physics_;
    GraphicsComponent graphics_;
};
```
这样，实例就可以是任何实现了抽象InputComponent接口的类型，其实例化只需输入需要的组件即可。而且修改功能时，无需改动其他组件。**代价**就是函数调用慢一些。
```cpp
Bjorn* bjorn=new Bjorn(new PlayerInputComponent());
```
物理和图像同样如此。最终合成一个通用的GameObject类。
```cpp
class PhysicsComponent{
public:
    virtual ~PhysicsComponent(){}
    virtual void update(GameObject& obj, World& world)=0;
};
class GraphicsComponent{
public:
    virtual ~GraphicsComponent(){}
    virtual void update(GameObject& obj, Graphics& graphics)=0;
}
```
```cpp
// 通用游戏对象
class GameObject{
public:
    int x, y;
    int velocity;

    GameObject(InputComponent* input,
               PhysicsComponent* physics,
               GraphicsComponent* graphics)
    : input_(input),
      physics_(physics),
      graphics_(graphics)
    {}

    void update(){
        input_->update(*this);
        physics_->update(*this, world);
        graphics_->update(*this, graphics);
    }

private:
    InputComponent* input_;
    PhysicsComponent* physics_;
    GraphicsComponent* graphics_;
};
```
而原本的Bjorn就无需建立具体的类，而是各具体类合并实现。
```cpp
GameObject* creatBjorn(){
    return new GameObject(new PlayerInputComponent(),       // 输入组建的具体类
                          new BjornPhysicsComponent(),      // 物理组件的具体类
                          new BjornGraphicsComponent());    // 图形组件的具体类
}
```
## 组件间通信
### 共享状态
状态可被容器内不同组件改变，且组件间保持解耦。非必要的数据（如渲染的图形）放在容器类中，会导致无意义的**内存损耗**。而且还需要保证组件运行的**顺序**（一般是输入、物理、渲染）正确有效。比如位置和大小就是适合作为共享状态。
### 相互引用
组件间相互通信，无需通过容器类。这种方式简单快捷，但会让组件绑在一起。比如动画和渲染，输入与AI，或物理与粒子等一堆分离的组件直接引用可能更简单。
```cpp
class BjornGraphicsComponent{
public:
    BjornGraphicsComponent(BjornPhysicsComponent* physics)
    : physics_(physics)
    {}

    void update(GameObject& obj, Graphics& graphics){
        Sprite* sprite;
        if(!physics.isOnGround()) sprite=&spriteJump_;
        else{
            // 现存的图形代码...
        }
        graphics.draw(*sprite, obj.x, obj.y);
    }

private:
    BjornPhysicsComponent* physics_;        // 与物理组件绑定

    Sprite spriteStand_;
    Sprite spriteWalkLeft_;
    Sprite spriteWalkRight_;
    Sprite spriteJump_;
};
```
### 发送消息
在容器类中建立一个消息系统，允许组件相互发送消息。这种方式中的同级组件解耦，容器类也简单。比如发生碰撞后发送信息让音乐组件播放这种发送后不管的就很适用。
```cpp
// 组件基类
class Component{
public:
    virtual ~Component(){};
    virtual void receive(Message message)=0;
};
```
```cpp
// 对象容器
class ContainerObject{
public:
    void send(Message message){
        for(int i=0;i<MAX_COMPONENTS;++i){
            if(components_[i]!=NULL){
                components_[i]->receive(message);       // 每个组件都接收消息
            }
        }
    }

private:
    static const int MAX_COMPONENTS=10;
    Component* components_[MAX_COMPONENTS];
};
```
三种方式各有优势，在实际中都可能用到。