# 状态模式
## 状态机
### 介绍
状态机在AI和编辑器程序中很出名，有基础的有限状态机，以及层次状态机和下推状态机。
### 问题
假设一个卷轴平台游戏，实现玩家在游戏世界中操纵的女英雄，她需要对玩家的输入做出响应。假设按B键她会跳跃。
```cpp
void Heroine::handleInput(Input input){
    if(input==PRESS_B){
        yVelocity_=JUMP_VELOCITY;
        setGraphics(IMAGE_JUMP);
    }
}
```
发现可以“空中跳跃”，修复方法就是给Heroine加isJumping的布尔字段。
```cpp
void Heroine::handleInput(Input input){
    if(input==PRESS_B){
        if(!isJumping_){
            isJumping_=true;
            // 跳跃...
        }
    }
}
```
接下来是长按下键角色卧倒（角色得在地上）。
```cpp
void Heroine::handleInput(Input input){
    if(input==PRESS_B){
        // 如果没在跳，就跳跃...
    }
    else if(input==PRESS_DOWN){
        if(!isJumping_){
            setGraphics(IMAGE_DUCK);
        }
    }
    else if(input==RELEASE_DOWN){
        setGraphics(IMAGE_STAND);
    }
}
```
结果得到了，长按下键卧倒，按B键跳起来，然后在空中松开下键，角色就可以神奇得站在空中了。修复方法很简单，添加另一个标识。
```cpp
void Heroine::handleInput(Input input){
    if(input==PRESS_B){
        if(!isJumping_&&isDucking_){
            // 跳跃...
        }
    }
    else if(input==PRESS_DOWN){
        if(!isJumping_){
            isDucking_=true;
            // 卧倒...
        }
    }
    else if(input==RELEASE_DOWN){
        if(!isDucking_){
            isDucking_=false;
            // 站立...
        }
    }
}
```
然后是角色在空中时按下键实现跳斩攻击。
```cpp
void Heroine::handleInput(Input input){
    if(input==PRESS_B){
        if(!isJumping_&&isDucking_){
            // 跳跃...
        }
    }
    else if(input==PRESS_DOWN){
        if(!isJumping_){
            isDucking_=true;
            // 卧倒...
        }
        else{
            isJumping_=false;
            setGraphics(IMAGE_DIVE);
        }
    }
    else if(input==RELEASE_DOWN){
        if(!isDucking_){
            isDucking_=false;
            // 站立...
        }
    }
}
```
结果是，跳斩（下坠）的过程中又可以跳跃了。然后有需要一个布尔字段。如此一来，你会得到一堆布尔字段以及一大堆if判断。
### 有限状态机
有限状态机（FSMs），其要点是：
1. 集合中包含所有可能的状态，比如站立、跳跃、俯卧和速降；
2. 状态机同时只能有一种状态；
3. 一连串的输入或事件被发送给状态机；
4. 每个状态都有一系列的转移，每个转移与输入和另一个状态相关。

所以需要的不是布尔字段，而是**枚举**或**枚举类**。通过状态进行**分支**，然后处理输入。
```cpp
enum State{
    STATE_STANDING,
    STATE_JUMPING,
    STATE_DUCKING,
    STATE_DIVING
};
```
```cpp
void Heroine::handleInput(Input input){
    switch(state_){
        case STATE_STANDING:
            if(input==PRESS_B){
                state_=STATE_JUMPING;
                yVelocity_=JUMP_VELOCITY;
                setGraphics(IMAGE_JUMP);
            }
            else if(input==PRESS_DOWN){
                state_=STATE_DUCKING;
                setGraphics(IMAGE_DUCK);
            }
            break;

        case STATE_JUMPING:
            if(input==PRESS_DOWN){
                state_=STATE_DIVING;
                setGraphics(IMAGE_DIVE);
            }
            break;

        case STATE_DUCKING:
            if(input==PELEASE_DOWN){
                state_=STATE_STANDING;
                setGraphics(IMAGE_STAND);
            }
            break;
    }
}
```
使用状态机后，一切都简洁明了了。于是乎增加一个新动作，英雄俯卧一段时间后充能，之后释放一次特殊攻击。
```cpp
void Heroine::update(){
    if(state_==STATE_DUCKING){
        chargeTime_++;      // 充能...
        if(chargeTime_>MAX_CHARGE){
            superBomb();
        }
    }
}
```
```cpp
void Heroine::handleInput(Input input){
    switch(state_){
        case STATE_STANDING:
            if(input==PRESS_DOWN){
                state_=STATE_DUCKING;
                chargeTime_=0;          // 重置充能时间
                setGraphics(IMAGE_DUCK);
            }
    }
}
```
但这样就需要修改两个方法，无法做到相关代码和数据**应该**待在同一地方。
## 状态模式
使用**虚函数**调用，而非switch。
### 状态接口
switch的每一处都成为了接口中的虚方法。
```cpp
// 状态基类
class HeroineState{
public:
    virtual ~HeroineState(){}
    virtual void handleInput(Heroine& heroine, Input input){}
    virtual void update(Heroine& heroine){}
};
```
### 状态类
将特殊攻击的实现包含在一个状态类当中。这样，相关代码和数据就在**同一地方**了。
```cpp
class DuckingState:public HeroineState{
public:
    DuckingState()
    : chargeTime_(0)
    {}

    virtual void handleInput(Heroine& heroine, Input input){
        if(input==RELEASE_DOWN){
            // 改回站立状态...
            heroine.setGraphics(IMAGE_STAND);
        }
    }

    virtual void update(Heroine& heroine){
        chargeTime_++;
        if(chargeTime_>MAX_CHARGE){
            heroine.superBomb();
        }
    }

private:
    int chargeTime_;
};
```
### 状态委托
使用指向**当前状态**的指针，代替庞大的switch。只需要将state_声明成不同的状态对象，就能改变状态。
```cpp
class Heroine{
public:
    virtual void handleInput(Input input){
        state_->handleInput(*this, input);
    }

    virtual void update(){
        state_->update(*this);
    }

private:
    HeroineState* state_;
};
```
## 状态对象
state_需要指向状态的实例，还需要在状态间转换。
### 静态状态
多个FSM可以同时在同一状态上运行，state_指向同一实例。
```cpp
class HeroineState{
public:
    static StandingState standing;
    static JumpingState jumping;
    static DuckingState ducking;
    static DivingState diving;

    // 其他代码...
};
```
```cpp
class StandingState:public HeroineState{
public:
    virtual void handleInput(Heroine& heroine, Input input){
        if(input==PRESS_B){
            heroine.state_=&Heroine::Jumping;       // 指向静态状态
            heroine.setGraphics(IMAGE_JUMP);
        }
    }

    // 其他代码...
};
```
### 实例化状态
分配**新状态**需要释放**当前状态**，通过handleInput的返回值实现。
```cpp
void Heroine::handleInput(Input input){
    HeroineState* state=state_->handleInput(*this, input);          // 返回一个新状态
    if(state!=nullptr){
        delete state_;      // 释放当前状态
        state_=state;
    }
}
```
```cpp
HeroineState* StandingState::handleInput(Heroine& heroine, Input input){
    if(input==PRESS_DOWN){
        // 俯卧...
        return new DurkingState();
    }
    return nullptr;     // 保持当前状态
}
```
## 入口和出口
每个状态通过入口行为设置自己的贴图。这样避免不同状态相互修改对方的贴图，实现状态间的解耦。
```cpp
class Standing:public HeroineState{
public:
    virtual void enter(Heroine& heroine){       // 入口行为
        heroine.setGraphics(IMAGE_STAND);
    }

    // 其他代码...
};
```
```cpp
void Heroine::handleInput(Input input){
    HeroineState* state=state_->handleInput(*this, input);
    if(state!=nullptr){
        delete state_;
        state_=state;
        state_->enter(*this);       // 调用新状态的入口行为
    }
}
```
当然，除了入口行为还有出口行为。比如，英雄在跳跃或跳斩后落体进入站立状态。只需重复相同的代码扩展出口行为，使角色离开当前状态。
## 并发状态机
通过多个状态机避免状态组合导致复杂和冗余。英雄获得武器系统，并可以在之前所有行为的过程中开火。通过两个状态机分别表示她**做的**和她**携带的**，避免因状态组合导致状态类的翻倍。当两个集合几乎没什么**联系**时，并发状态机会工作得不错。
```cpp
class Heroine{
    // 其他代码...
private:
    HeroineState* state_;
    HeroineState* equipment_;
};
```
```cpp
void HeroineState::handleInput(Input input){
    state_->handleInput(*this, input);
    equipment_->handleInput(*this, input);
}
```
## 分层状态机
通过**父子状态**（状态栈）来实现更多相似的状态（如站立、行走、奔跑、滑铲等），实现状态间重用。
```cpp
class OnGroundState:public HeroineState{        // 基类父状态
public:
    virtual void handleInput(Heroine& heroine, Input input){
        if(input==PRESS_B){
            // 跳跃...
        }
        else if(input==PRESS_DOWN){
            // 俯卧...
        }
    }
};
```
```cpp
class DuckingState:public OnGroundState{
    virtual void handleInput(Heroine& heroine, Input input){
        if(input==RELEASE_DOWN){
            // 站立...
        }
        else{
            // 没有处理输入，返回上一层
            OnGroundState::handleInput(heroine, input);
        }
    }
};
```
## 下推自动机
上述状态没有**历史**的概念，即当前状态并不知道**曾在**什么状态，无法简单的回到上一状态。比如，射击后返回之前状态。**存储**和**回想**就是**下推自动机**的功能。通过**栈**来实现：新状态可以**压入**栈中；状态结束后可以**弹出**，其下面的状态（原状态）会成为新状态。