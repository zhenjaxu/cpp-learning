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
#### Board
Board作为背景，即已经固定的方块，不需要一直更新，但需要一直绘制。通过mGrid标记已经固定的方块，并设置相应的颜色编号，共7种颜色。
```cpp
public:
void Draw(SDL_Renderer* renderer) override;
private:
std::vector<std::vector<int>> mGrid;
const SDL_Color mColors[7];
```
Board还需实现基本功能，即碰撞检测、消除行、重置等等。
```cpp
bool IsValid(const Vector2 blocks[4]) const;
void Lock(const Vector2 blocks[4], int type);
void ClearLines();
void Reset();
```
碰撞检测借助mGrid查看Piece传入的下一帧blocks所在处是否超界，或与mGrid标记的地方（即锁住的方块）重合。
```cpp
bool Board::IsValid(const Vector2 blocks[4]) const {
    for(int i=0;i<4;++i){
        if(blocks[i].x<0||
           blocks[i].x>=mColumns||
           blocks[i].y>=mRows) return false;
        if(blocks[i].y>=0&&
           mGrid[blocks[i].y][blocks[i].x]!=-1) return false;
    }
    return true;
}
```
方块的锁定则直接将blocks在mGrid上标记即可。
```cpp
void Board::Lock(const Vector2 blocks[4], int type){
    for(int i=0;i<4;++i){
        if(blocks[i].y>=0) mGrid[blocks[i].y][blocks[i].x]=type;
    }
}
```
当行满时，进行消除并将上面的行向下移。通过erase删除对应行，并将开头插入一行空值。
```cpp
void Board::ClearLines(){
    for(int y=mRows-1;y>=0;--y){
        bool full=true;
        for(int x=0;x<mColumns;++x){
            if(mGrid[y][x]==-1){
                full=false;
                break;
            }
        }
        if(full){
            mGrid.erase(mGrid.begin()+y);
            mGrid.insert(mGrid.begin(), std::vector<int>(mColumns, -1));
            ++y;
        }
    }
}
```
重置则清空mGrid即可，使用assign重新赋值。
```cpp
void Board::Reset(){
    mGrid.assign(mRows, std::vector<int>(mColumns, -1));
}
```
#### Piece
Piece作为玩家控制的方块，需要对输入进行处理，还需要一直更新并绘制。
```cpp
void Update(float deltaTime) override;
void ProcessInput(const uint8_t* keyState) override;
void Draw(SDL_Renderer* renderer) override;
```
Piece由mBlocks构成，其不同的值对应不同的形状，从常量SHAPES直接获取。
```cpp
const Vector2 SHAPES[7][4]={
    {{0,-1},{0,0},{0,1},{0,2}},
    {{-1,0},{0,0},{1,0},{0,1}},
    {{0,0},{1,0},{0,1},{1,1}},
    {{0,-1},{0,0},{0,1},{1,1}},
    {{0,-1},{0,0},{0,1},{-1,1}},
    {{1,0},{0,0},{0,1},{-1,1}},
    {{-1,0},{0,0},{0,1},{1,1}}
};
```
Piece在绘制时，可通过GetGame()->GetBoard()获取Board对象，然后通过GetColors获取对应颜色。
```cpp
const SDL_Color* GetColors() const {return mColors;}
```
## 编译运行
在终端上编译调试程序。
```shell
cmake -G "MinGW Makefiles" -B build
cmake --build build
```
确保build中有SDL2.dll，运行游戏。
```shell
./build/Tetris
```
游戏启动和退出正常，方块的移动、旋转、软降控制正常，方块的锁定和行消除正常，方块生成和绘制正常。运行部分截图如下。
![1782972506405](image/README/1782972506405.png)