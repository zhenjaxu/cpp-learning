#pragma once
#include<SDL2/SDL.h>

struct Position{
    float x, y;
};

class Game{
public:
    Game()
    : mIsRunning(true),
      mTicksCount(0)
    {}

    bool Initialize(){
        int sdlResult=SDL_Init(SDL_INIT_VIDEO);
        if(sdlResult){
            SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
            return false;
        }

        mWindow=SDL_CreateWindow(
            "Pong (Chapter 1)",
            100, 100,
            1024, 768,
            0
        );
        if(!mWindow){
            SDL_Log("Failed to create window: %s", SDL_GetError());
            return false;
        }

        mRenderer=SDL_CreateRenderer(
            mWindow,
            -1,
            SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC
        );
        if(!mRenderer){
            SDL_Log("Failed to create renderer: %s", SDL_GetError());
            SDL_DestroyWindow(mWindow);
            return false;
        }

        return true;
    }

    void RunLoop(){
        while(mIsRunning){
            ProcessInput();
            UpdateGame();
            GenerateOutput();
        }
    }

    void Shutdown(){
        SDL_DestroyRenderer(mRenderer);
        SDL_DestroyWindow(mWindow);
        SDL_Quit();
    }

private:
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();

    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;
    bool mIsRunning;

    const int thickness=15;
    const int paddleH=100;
    Position mBallPos{512, 384};
    Position mPaddlePos{20, 384};

    Position mBallVel{-400.0f, 435.0f};
    int mPaddleDir;

    Uint32 mTicksCount;
};

void Game::GenerateOutput(){
    SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
    SDL_RenderClear(mRenderer);

    SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);

    SDL_Rect wall1{0, 0, 1024, thickness};
    SDL_Rect wall2{1024-thickness, 0, thickness, 768};
    SDL_Rect wall3{0, 768-thickness, 1024, thickness};

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
        paddleH
    };

    SDL_RenderFillRect(mRenderer, &wall1);
    SDL_RenderFillRect(mRenderer, &wall2);
    SDL_RenderFillRect(mRenderer, &wall3);
    SDL_RenderFillRect(mRenderer, &ball);
    SDL_RenderFillRect(mRenderer, &paddle);
    
    SDL_RenderPresent(mRenderer);
}

void Game::ProcessInput(){
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        switch(event.type){
            // If this is an SDL_QUIT event, end loop
            case SDL_QUIT:
                mIsRunning=false;
                break;
        }
    }

    // Get state of keyboard
    const Uint8* state=SDL_GetKeyboardState(NULL);
    // If escap is pressed, alse end loop
    if(state[SDL_SCANCODE_ESCAPE]){
        mIsRunning=false;
    }
    
    mPaddleDir=0;
    if(state[SDL_SCANCODE_W]){
        mPaddleDir-=1;
    }
    if(state[SDL_SCANCODE_S]){
        mPaddleDir+=1;
    }
}

#include<cmath>

void Game::UpdateGame(){
    while(!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount+16));
    float deltaTime=(SDL_GetTicks()-mTicksCount)/1000.0f;
    mTicksCount=SDL_GetTicks();

    if(deltaTime>0.05f) deltaTime=0.05f;

    if(mBallPos.y<thickness&&mBallVel.y<0.0f||
       mBallPos.y>(768.0f-thickness)&&mBallVel.y>0.0f){
        mBallVel.y*=-1;
    }
    if(mBallPos.x>(1024.0f-thickness)&&mBallVel.x>0.0f){
        mBallVel.x*=-1;
    }

    if(std::abs(mBallPos.y-mPaddlePos.y)<paddleH/2.0f&&
       mBallPos.x<35.0f&&mBallPos.x>12.5f&&
       mBallVel.x<0.0f){
        mBallVel.x*=-1;
    }

    if(mPaddleDir!=0){
        mPaddlePos.y+=mPaddleDir*300.0f*deltaTime;

        if(mPaddlePos.y<(paddleH/2.0f+thickness)){
            mPaddlePos.y=paddleH/2.0f+thickness;
        }else if(mPaddlePos.y>(768.0f-paddleH/2.0f-thickness)){
            mPaddlePos.y=768.0f-paddleH/2.0f-thickness;
        }
    }

    mBallPos.x+=mBallVel.x*deltaTime;
    mBallPos.y+=mBallVel.y*deltaTime;

    if(mBallPos.x<0){
        mBallPos.x=512;
        mBallPos.y=384;
        mBallVel.x*=-1;
    }
}