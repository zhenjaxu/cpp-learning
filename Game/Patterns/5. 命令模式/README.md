# 命令模式
## 介绍
命令是具现化（将**概念**变成**数据**）的方法调用，命令模式一种回调的面向对象实现。
## 模式
对于一些没有闭包**函数**的语言，命令模式通过**类**模拟闭包。
### 配置输入
每个游戏都有一块代码读取用户的输入（如按键按下、键盘敲击、鼠标点击）。
```cpp
void InputHandler::handleInput(){
    if(isPressed(BUTTON_X)) jump();
    else if(isPressed(BUTTON_Y)) fireGun();
    else if(isPressed(BUTTON_A)) swapWeapon();
    else if(isPressed(BUTTON_B)) lurchIneffectively();
}
```
通过基类指针实现输入的个性化**配置**。
```cpp
// 命令基类
class Command{
public:
    virtual ~Command(){}
    virtual void execute()=0;
};
```
```cpp
// 跳跃命令
class JumpCommand:public Command{
public:
    virtual void execute(){jump();}     // 执行跳跃
};
// 开火命令
class FireCommand:public Command{
    virtual void execute(){fireGun();}
};
```
```cpp
class InputHandler{
public:
    void handleInput();

    // 绑定按键方法...

private:
    // 各按键
    Command* buttonX_;
    Command* buttonY_;
    Command* buttonA_;
    Command* buttonB_;
};
```
```cpp
// 处理按键绑定的指令
void InputHandler::handleInput{
    if(isPressed(BUTTON_X)) buttonX_->execute();
    else if(isPressed(BUTTON_Y)) buttonY_->execute();
    else if(isPressed(BUTTON_A)) buttonA_->execute();
    else if(isPressed(BUTTON_B)) buttonB_->execute();
};
```
### 角色说明
将角色作为参数，实现指定角色的处理。
```cpp
class Command{
public:
    virtual ~Command(){}
    virtual void execute(GameActor& actor)=0;
};
```
```cpp
class JumpCommand{
public:
    virtual void execute(GameActor& actor){
        actor.jump();
    }
};
```
让handleInput返回指令，玩家就可以控制游戏中的任何角色，只要在参数中传入对应角色。
```cpp
Command* InputHandler::handleInput(){
    if(isPressed(BUTTON_X)) return buttonX_;
    if(isPressed(BUTTON_Y)) return buttonY_;
    if(isPressed(BUTTON_A)) return buttonA_;
    if(isPressed(BUTTON_B)) return buttonB_;

    return nullptr;
}
```
```cpp
Command* command=inputHandler.handleInput();
if(command) command->execute(actor);
```
通过AI生成命令操作游戏中的其他角色，也可以为玩家角色加上AI，实现自动演示功能。
### 撤销与重做
**撤销**一个误操作是游戏中非常重要的功能，而命令模式实现它非常容易。
```cpp
class Command{
public:
    virtual ~Command(){}
    virtual void execute()=0;
    virtual void undo()=0;          // 撤销
};
```
```cpp
class MoveUnitCommand:public Command{
public:
    MoveUnitCommand(Unit* unit, int x, int y)
    : unit_(unit),
      xBefore_(0),
      yBefore_(0),
      x_(x),
      y_(y)
    {}

    virtual void execute(){
        // 移动前保存位置
        xBefore_=unit_->x();
        yBefore_=unit_->y();

        unit_->moveTo(x_, y_);
    }

    virtual void undo(){
        unit_->moveTo(xBefore_, yBefore_);
    }

private:
    Unit* unit_;
    int xBefore_, yBefore_;
    int x_, y_;
};
```
```cpp
Command* handleInput(){
    Unit* unit=getSelectedUnit();
    
    if(isPressed(BUTTON_UP)){
        // 向上移动单位
        int destY=unit->y()+1;
        return new MoveUnitCommand(unit, unit->x(), destY);
    }

    if(isPressed(BUTTON_DOWN)){
        // 向下移动单位
        int destY=unit->y()-1;
        return new MoveUnitCommand(unit, unit->x(), destY);
    }

    // 其他的移动...

    return nullptr;
}
```
将返回的命令放入栈中，可实现连续回滚。需要回滚时，弹出栈顶命令，执行undo实现撤销。