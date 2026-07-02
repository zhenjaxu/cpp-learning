# SDL2
## 介绍
SDL2是一个开源、跨平台的多媒体开发库，主要用于提供对音频、视频、键盘、鼠标等硬件的底层访问接口，广泛用于游戏开发。
## SDL2安装
安装包网址：https://github.com/libsdl-org/SDL/releases/tag/release-2.30.12
## SDL2测试
安装包解压至D:/SDL2/，并将里面的include、bin、lib三个复制到SDL2中。
构建cmake工程，编写CMakeLists.txt。
```cmake
cmake_minimum_required(VERSION 3.10)
project(SDL2_test)

set(CMAKE_CXX_STANDARD 17)

# SDL2 路径
set(SDL2_INCLUDE_DIR "D:/SDL2/include")
set(SDL2_LIB_DIR "D:/SDL2/lib")

include_directories(${SDL2_INCLUDE_DIR})
link_directories(${SDL2_LIB_DIR})

add_executable(test main.cpp)

# Windows MinGW 链接顺序: SDL2main, SDL2
target_link_libraries(test SDL2main SDL2)
```
包含SDL路径，并取消SDL对main的宏定义。
```cpp
#include<SDL2/SDL.h>
#undef main     // 取消SDL宏定义
```
依次初始化SDL、window、renderer，并检查。
```cpp
if(SDL_Init(SDL_INIT_VIDEO)<0){
    std::cerr<<"SDL_Init Error: "<<SDL_GetError()<<std::endl;
    return 1;
}

SDL_Window* window=SDL_CreateWindow(
    "Hello SDL2",
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    800, 600, SDL_WINDOW_SHOWN
);
if(!window){
    std::cerr<<"Window Error: "<<SDL_GetError()<<std::endl;
    SDL_Quit();
    return 1;
}

SDL_Renderer* renderer=SDL_CreateRenderer(window, -1, 
    SDL_RENDERER_ACCELERATED);
if(!renderer){
    std::cerr<<"Renderer Error: "<<SDL_GetError()<<std::endl;
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
}
```
游戏循环，在黑色屏幕中间生成一个红方块。
```cpp
bool running = true;
SDL_Event event;

while(running){
    while(SDL_PollEvent(&event)){
        if(event.type==SDL_QUIT) running=false;
    }

    // 黑色背景
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);     // 设置画笔颜色
    SDL_RenderClear(renderer);                          // 用画笔清空整个屏幕

    // 画红色方块（屏幕中央 50x50）
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect rect={375, 275, 50, 50};                   // 定义一个矩形
    SDL_RenderFillRect(renderer, &rect);                // 用画笔填充矩形

    SDL_RenderPresent(renderer);                        // 将画好的内容一次性展示到窗口
}

SDL_DestroyRenderer(renderer);
SDL_DestroyWindow(window);
SDL_Quit();
```
在终端测试。在build中生成cmake文件，编译生成test.exe。
```shell
cmake -G "MinGW Makefiles" -B build
cmake --build build
```
需要将SDL2中bin里面的SDL2.dll复制进build中，然后开始测试。
```shell
./build/test
```
成功打开并显示出红方块。