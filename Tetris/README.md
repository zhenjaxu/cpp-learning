# 俄罗斯方块
## 介绍
基于SDL2实现的经典俄罗斯方块游戏，包含移动、降落、消除等基本功能。
## 技术栈
使用C++语言编写，借助SDL2接口实现游戏基本功能，并采用CMake工具编译。
## 代码实现
### 基本架构
Game类实现游戏的初始化、循环和退出。而游戏的循环有包括输入处理、游戏更新以及图形渲染。
```cpp
public:
bool Initialize();
void RunLoop();
void Shutdown();
private:
void ProcessInput();
void UpdateGame();
void GenerateOutput();
```
Actor类对游戏对象进行管理和实现。通过虚函数的方式，实现多态。
```cpp
public:
virtual void Update(float deltaTime);
virtual void ProcessInput(const uint8_t* keyState);
virtual void Draw(SDL_Renderer* renderer);
```
在Game类中还会提供Actor对象的增删方法，并在Actor对象构造和析构的时候调用，实现资源的自动管理。
```cpp
void AddActor(class Actor* actor);
void RemoveActor(class Actor* actor);
```
```cpp
Actor::Actor(Game* game)
:mGame(game)
{
    mGame->AddActor(this);
}

Actor::~Actor(){
    mGame->RemoveActor(this);
}
```
### 逻辑实现
游戏可拆分为两个部分，Board作为背景，Piece作为玩家操作的方块。
```cpp

```