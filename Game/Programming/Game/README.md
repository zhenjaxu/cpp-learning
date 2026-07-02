# 游戏循环和Game类
## 开发环境
以VS Code（mingW64）为开发环境，SDL2为游戏开发库，并使用CMake进行工程构建和管理。
## 游戏循环
每一秒都需要对游戏进行**多次更新**，而游戏循环是更新游戏的**循环**。
```cpp
void Game::RunLoop(){
    while(!mShouldQuit){
        // Process Inputs
        JoystickData j=GetJoystickData();

        // Update Game World
        UpdatePlayerPosition(j);

        for(Ghost& g:mGhost){
            if(g.Collides(player)){
                // Handle Pac-Man colliding with a ghost...
            }else{
                g.Update();
            }
        }

        // Handle Pac-Man eating pellets
        // ...

        // Generate Outputs
        RenderGraphics();
        RenderAudio();
    }
}
```
## Game类
整个游戏有一个**骨架**，即Game类。其包含初始化、游戏循环、关闭等功能，而三个私有成员函数（ProcessInput、UpdateGame、GenerateOutput）则是游戏循环的**三要素**。
### 基本结构
```cpp
class Game{
public:
    Game();
    // Initialize the game
    bool Initialize();
    // Runs the game loop until the game is over
    void loop();
    // Shutdown the game
    void Shutdown();
private:
    // Helper functions for the game loop
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();
    // Window created by SDL
    SDL_Window* mWindow;
    // Game should continue to run
    bool mIsRuning
};
```
### 初始化
游戏的初始化在Initialize里完成，成功则返回true，否则返回false。首先是**SDL库**的初始化，标志位包含：
1. SDL_INIT_AUDIO 音频设备管理、回放和录音；
2. SDL_INIT_VIDEO 创建窗口的视频系统，与OpenGL库和2D图形相连接；
3. SDL_INIT_HAPTIC 力反馈子系统；
4. SDL_INIT_GANECONTROLLER 支持控制器输入设备的子系统。

其初始化结果为整型，返回0表示**成功**，非0表示失败。
```cpp
int sdlResult=SDL_Init(SDL_INIT_VIDEO);     // Return int, return 0(success)

if(sdlResult!=0){
    SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
    return false;
}
```
**窗口**的初始化接收多个参数：窗口标题、左上角的x/y坐标、窗口的宽度/高度，以及可选的任何窗口创建标志。其标志位包含：
1. 0 无要求；
2. SDL_WINDOW_FULLSCREEN　使用全屏模式；
3. SDL_WINDOW_FULLSCREEN_DESKTOP 在当前桌面分辨率下使用全屏模式（忽略宽度/高度参数）；
4. SDL_WINDOW_OPENGL 为OpenGL图形库添加支持；
5. SDL_WINDOW_RESIZEABLE 允许用户调整窗口大小。

初始化结果保存在成员变量mWindow（SDL_Window的指针类型）中。如果失败，指针置空。
```cpp
mWindow=SDL_CreateWindow(
    "Game Programming in C++ (Chapter 1)",  // Window title
    100,        // Top left x-coordinate of window
    100,        // Top left y-coordinate of window
    1024,       // Width of window
    768,        // Height of window
    0           // Flags (0 for no flags set)
);

if(!mWindow){
    SDL_Log("Failed to create window: %s", SDL_GetError());
    return false;
}
```
### 关闭
关闭游戏时则销毁窗口并关闭SDL库。
```cpp
void Game::Shutdown(){
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}
```
### 游戏循环
mIsRunning查看游戏是否运行。循环内**按顺序**执行用户输入处理、游戏物理更新、绘制和渲染。
```cpp
void Game::RunLoop(){
    while(mIsRunning){
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}
```
### main函数
main.cpp作为程序入口，包含Game.hpp。其内创建Game对象，调用其成员函数。
```cpp
int main(int argc, char** argv){
    Game game;
    bool success=game.Initialize();
    if(success){
        game.RunLoop();
    }
    game.Shutdown();
    return 0;
}
```
此时，编译运行游戏，成功创建一个空白窗口。
```cpp
cmake -G "MinGW Makefiles" -B build
cmake --build build
./build/game
```
### 基本输入处理
SDL库管理一个从操作系统接收事件的内部队列。除了SDL自动处理的事件（如移动窗口），其他事件（如按键的输入）处理需在ProcessInput中实现。SDL_PollEvent用于获取队列中的事件，成功后会返回true。
```cpp
SDL_Event event;
// While there are still events in the queue
while(SDL_PollEvent(&event)){
    // Handle different event types here
    switch(event.type){
        case SDL_QUIT:
            mIsRunning=false;
            break;
        // Other events...
    }
}
```
SDL_GetKeyboardState获取键盘的整个状态，返回状态数组的指针。
```cpp
const Uint8* state=SDL_GetKeyboardState(NULL);      // Return an array pointer
if(state[SDL_SCANCODE_ESCAPE]){
    mIsRunning=false;
}
```
### 2D图形
渲染器renderer的初始化和销毁。其初始化参数包含：窗口指针、默认值、标志位。只用一个窗口时，默认值为-1。标志位包含：
1. SDL_RENDERER_ACCELERATED 图形加速；
2. SDL_RENDERER_PRESENTVSYNC 垂直同步（双缓冲）。

```cpp
SDL_Renderer* mRenderer;
```
```cpp
mRenderer=SDL_CreateRenderer(
    mWindow,        // Window to create renderer for
    -1,             // Usually -1 (Just one window)
    SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC
);
```
```cpp
SDL_DestroyRenderer(mRenderer);
```
基本绘制由SDL_SetRenderDrawColor、SDL_RenderClear、SDL_RenderPresent完成。除了渲染器指针，颜色设置还需传入RGBA四元组设置颜色和透明度。
```cpp
SDL_SetRenderDrawColor(
    mRenderer,
    0,      // R
    0,      // G
    255,    // B
    255     // A
);
```
```cpp
SDL_RenderClear(mRenderer);
```
```cpp
SDL_RenderPresent(mRenderer);
```
## 游戏Pong
Pong作为游戏编程的helloWorld。其中，球在屏幕上移动，玩家控制球拍击打球。
### 绘制图形
使用渲染器绘制墙壁、球和球拍。通过SDL_Rect结构体指定位置（左上角）和尺寸。
```cpp
SDL_Rect wall{
    0,           // Top left x
    0,           // Top left y
    1024,        // Width
    thickness   // Height (const int (15))
};
```
设置渲染器颜色并用SDL_RenderFillRect填充矩形。填充矩形的参数传入矩形的**地址**。
```cpp
SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
SDL_RenderFillRect(mRenderer, &wall);
```
使用Position类型的成员变量存放球和球拍的位置。球和球拍都以中心点为坐标，矩形创建的时候x和y需偏移。
```cpp
struct Position{
    float x, y;
};
```
```cpp
SDL_Rect ball{
    static_cast<int>(mBallPos.x-thickness/2),
    static_cast<int>(mBallPos.y-thickness/2),
    thickness,
    thickness
};
SDL_Rect paddle{
    static_cast<int>(mPaddlePos.x-thickness/2),
    static_cast<int>(mPaddlePos.y-paddleH/2),
    thickness,
    paddleH           // Height of the paddle (const int (100))
};
```
### 更新游戏
该种更新方式固定更新步长，不同处理器上移动速度**不同**。
```cpp
// Update x position by 5 pixels
enemy.mPosition.x+=5;
```
该种更新方式根据**增量时间**动态更新，不同处理器上移动速度**一致**。
```cpp
// Update x position by 150 pixels/second
enemy.mPosition.x+=150*deltaTime;
```
成员变量mTicksCount（初始化时为0）记录时间（单位ms）。通过SDL_GetTicks获取当前帧时间并减去上一帧记录的时间，得到增量时间deltaTime（单位s）。同时防止暂停等操作导致的增量时间过大，游戏跨度也会很大。在处理器提前完成该帧任务后进入休息，直到帧间隔时间过完后进入下一帧的游戏更新。
```cpp
Uint32 mTicksCount;
```
```cpp
// Delta time is the difference in ticks from last frame
// (converted to seconds)
float deltaTime=(SDL_GetTicks()-mTicksCount)/1000.0f;
// Update tick counts (for next frame)
mTicksCount=SDL_GetTicks();
// Clamp maximum delta time value
if(deltaTime>0.05f) deltaTime=0.05f;

// TODO: Update objects in game world as function of delta time!
// ...

// Wait until 16ms has elapsed since last frame
while(!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount+16));
```
根据按键输入，更新球拍位置。球拍移动方向在输入处理中实现，球拍位置更新在游戏更新中实现。
```cpp
mPaddleDir=0;
if(state[SDL_SCANCODE_W]){
    mPaddleDir-=1;
}
if(state[SDL_SCANCODE_S]){
    mPaddleDir+=1;
}
```
```cpp
if(mPaddleDir!=0){
    mPaddlePos.y+=mPaddleDir*300.0f*deltaTime;
    // Make sure paddle doesn't move off screen!
    if(mPaddlePos.y<(paddleH/2.0f+thickness)){
        mPaddlePos.y=paddleH/2.0f+thickness;
    }else if(mPaddlePos.y>(768.0f-paddleH/2.0f-thickness)){
        mPaddlePos.y=768.0f-paddleH/2.0f-thickness;
    }
}
```
球的位置更新同样在游戏更新中，但需先检测球与球拍或墙的碰撞，确定其运动方向。
```cpp
mBallPos.x+=mBallVel.x*deltaTime;
mBallPos.y+=mBallVel.y*deltaTime;
```
```cpp
if(mBallPos.y<=thickness&&mBallVel.y<0.0f){
    mBallPos.y*=-1;
}
```
```cpp
if(
    // Our y-difference is small enough
    std::abs(mBallPos.y-mPaddlePos.y)<=paddleH/2.0f&&
    // Ball is at the correct x-position
    mBallPos.x<=35.0f&&mBallPos.x>=12.5f&&
    // The ball is moving to the left
    mBallVel.x<0.0f
){
    mBallVel.x*=-1;
}
```
### 游戏测试
在终端编译运行main文件。
```cpp
cmake -G "MinGW Makefiles" -B build
cmake --build build
./build/game
```
游戏成功运行，小球碰撞正常，球拍移动正常，小球出界后回到中心点。按ECS或退出窗口等能成功退出游戏。