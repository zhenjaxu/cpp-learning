#include"Game.hpp"
#include"SDL2/SDL_image.h"


Game::Game()
:mWindow(nullptr)
,mRenderer(nullptr)
,mIsRunning(true)
,mUpdatingActors(false)
{}

Game::Initialize(){
    if(SDL_Init(SDL_INIT_VIDEO||SDL_INIT_AUDIO)){
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    mWindow=SDL_CreateWindow("Travel In Space", 200, 100, 1024, 768, 0);
    if(!mWindow){
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    mRenderer=SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED||SDL_RENDERER_PRESENTVSYNC);
    if(!mRenderer){
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }

    if(!IMG_Init(IMG_INIT_PNG)){
        SDL_Log("Unable to initialize SDL_image: %s", SDL_GetError());
        return false;
    }

    Random::Init();
    LoadData();
    mTicksCount=SDL_GetTicks();

    return true;
}

