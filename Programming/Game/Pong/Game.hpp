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
    Position mPaddle1Pos{20, 384};
    Position mPaddle2Pos{1004, 384};

    Position mBallVel{-200.0f, 235.0f};
    int mPaddle1Dir;
    int mPaddle2Dir;

    Uint32 mTicksCount;
};

void Game::GenerateOutput(){
    SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
    SDL_RenderClear(mRenderer);

    SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);

    SDL_Rect wall1{0, 0, 1024, thickness};
    SDL_Rect wall3{0, 768-thickness, 1024, thickness};

    SDL_Rect ball{
        static_cast<int>(mBallPos.x-thickness/2),
        static_cast<int>(mBallPos.y-thickness/2),
        thickness,
        thickness
    };
    SDL_Rect paddle1{
        static_cast<int>(mPaddle1Pos.x-thickness/2),
        static_cast<int>(mPaddle1Pos.y-paddleH/2),
        thickness,
        paddleH
    };
    SDL_Rect paddle2{
        static_cast<int>(mPaddle2Pos.x-thickness/2),
        static_cast<int>(mPaddle2Pos.y-paddleH/2),
        thickness,
        paddleH
    };

    SDL_RenderFillRect(mRenderer, &wall1);
    SDL_RenderFillRect(mRenderer, &wall3);
    SDL_RenderFillRect(mRenderer, &ball);
    SDL_RenderFillRect(mRenderer, &paddle1);
    SDL_RenderFillRect(mRenderer, &paddle2);
    
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
    
    mPaddle1Dir=0;
    mPaddle2Dir=0;
    if(state[SDL_SCANCODE_W]){
        mPaddle1Dir-=1;
    }
    if(state[SDL_SCANCODE_S]){
        mPaddle1Dir+=1;
    }
    if(state[SDL_SCANCODE_I]){
        mPaddle2Dir-=1;
    }
    if(state[SDL_SCANCODE_K]){
        mPaddle2Dir+=1;
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

    if(std::abs(mBallPos.y-mPaddle1Pos.y)<paddleH/2.0f&&
       mBallPos.x<35.0f&&mBallPos.x>12.5f&&
       mBallVel.x<0.0f){
        mBallVel.x*=-1;
    }

    if(std::abs(mBallPos.y-mPaddle2Pos.y)<paddleH/2.0f&&
       mBallPos.x<1011.5f&&mBallPos.x>989.0f&&
       mBallVel.x>0.0f){
        mBallVel.x*=-1;
    }

    if(mPaddle1Dir!=0){
        mPaddle1Pos.y+=mPaddle1Dir*300.0f*deltaTime;

        if(mPaddle1Pos.y<(paddleH/2.0f+thickness)){
            mPaddle1Pos.y=paddleH/2.0f+thickness;
        }else if(mPaddle1Pos.y>(768.0f-paddleH/2.0f-thickness)){
            mPaddle1Pos.y=768.0f-paddleH/2.0f-thickness;
        }
    }

    if(mPaddle2Dir!=0){
        mPaddle2Pos.y+=mPaddle2Dir*300.0f*deltaTime;

        if(mPaddle2Pos.y<(paddleH/2.0f+thickness)){
            mPaddle2Pos.y=paddleH/2.0f+thickness;
        }else if(mPaddle2Pos.y>(768.0f-paddleH/2.0f-thickness)){
            mPaddle2Pos.y=768.0f-paddleH/2.0f-thickness;
        }
    }

    mBallPos.x+=mBallVel.x*deltaTime;
    mBallPos.y+=mBallVel.y*deltaTime;

    if(mBallPos.x<0||mBallPos.x>1024){
        mBallPos.x=512;
        mBallPos.y=384;
        mBallVel.x*=-1;
    }
}