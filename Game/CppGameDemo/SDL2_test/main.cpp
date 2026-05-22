#include<SDL2/SDL.h>
#undef main

#include<iostream>

int main(int argc, char* argv[]){
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

    bool running = true;
    SDL_Event event;

    while(running){
        while(SDL_PollEvent(&event)){
            if(event.type==SDL_QUIT) running=false;
        }

        // 黑色背景
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // 画红色方块（屏幕中央 50x50）
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect rect={375, 275, 50, 50};
        SDL_RenderFillRect(renderer, &rect);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}