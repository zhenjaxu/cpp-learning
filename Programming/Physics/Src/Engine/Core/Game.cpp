#include"Game.h"

#include"SDL2/SDL_image.h"
#include<algorithm>

#include"Actor.h"
#include"../Renderer/SpriteComponent.h"
#include"../Renderer/BGSpriteComponent.h"
#include"../../Game/Ship.h"
#include"../../Game/Asteroid.h"
#include"../Utils/Random.h"


Game::Game()
:mWindow(nullptr)
,mRenderer(nullptr)
,mIsRunning(true)
,mUpdatingActors(false)
{}

bool Game::Initialize(){
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

void Game::RunLoop(){
    while(mIsRunning){
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::Shutdown(){
    UnloadData();
    IMG_Quit();

    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}

void Game::ProcessInput(){
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        switch(event.type){
            case SDL_QUIT:
                mIsRunning=false;
                break;
        }
    }

    const Uint8* keyState=SDL_GetKeyboardState(NULL);
    if(keyState[SDL_SCANCODE_ESCAPE]){
        mIsRunning=false;
    }

    mUpdatingActors=true;
    for(auto actor:mActors){
        actor->ProcessInput(keyState);
    }
    mUpdatingActors=false;
}

void Game::UpdateGame(){
    while(!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount+16));
    float deltaTime=(SDL_GetTicks()-mTicksCount)/1000.0f;
    if(deltaTime>0.05f) deltaTime=0.05f;
    mTicksCount=SDL_GetTicks();

    mUpdatingActors=true;
    for(auto actor:mActors){
        actor->Update(deltaTime);
    }
    mUpdatingActors=false;

    for(auto pending:mPendingActors){
        mActors.emplace_back(pending);
    }
    mPendingActors.clear();

    std::vector<Actor*> deadActors;
    for(auto actor:mActors){
        if(actor->GetState()==Actor::EDead){
            deadActors.emplace_back(actor);
        }
    }

    for(auto actor:deadActors){
        delete actor;
    }
}

void Game::GenerateOutput(){
    SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
    SDL_RenderClear(mRenderer);

    for(auto sprite:mSprites){
        sprite->Draw(mRenderer);
    }

    SDL_RenderPresent(mRenderer);
}

void Game::LoadData(){
    mShip=new Ship(this);
    mShip->SetPosition(Vector2(100.0f, 384.0f));
    mShip->SetRotation(Math::PiOver2);
    mShip->SetScale(1.5f);

    const int numAsteroids=20;
    for(int i=0;i<numAsteroids;++i){
        new Asteroid(this);
    }

    Actor* temp=new Actor(this);
    temp->SetPosition(Vector2(512.0f, 384.0f));
    BGSpriteComponent* bg=new BGSpriteComponent(temp);
    bg->SetScreenSize(Vector2(1024.0f, 768.0f));
    std::vector<SDL_Texture*> bgtexs{
        GetTexture("Assets/Textures/Farback/Farback01.png"),
        GetTexture("Assets/Textures/Farback/Farback02.png")
    };
    bg->SetBGTextures(bgtexs);
    bg->SetScrollSpeed(-50.0f);

    bg=new BGSpriteComponent(temp, 50);
    bg->SetScreenSize(Vector2(1024.0f, 768.0f));
    bgtexs={
        GetTexture("Assets/Textures/Stars.png"),
        GetTexture("Assets/Textures/Stars.png")
    };
    bg->SetBGTextures(bgtexs);
    bg->SetScrollSpeed(-150.0f);
}

void Game::UnloadData(){
    while(!mActors.empty()){
        delete mActors.back();
    }

    for(auto i:mTextures){
        SDL_DestroyTexture(i.second);
    }
    mTextures.clear();
}

SDL_Texture* Game::GetTexture(const std::string& fileName){
    SDL_Texture* tex=nullptr;

    auto iter=mTextures.find(fileName);
    if(iter!=mTextures.end()){
        tex=iter->second;
    }else{
        SDL_Surface* surf=IMG_Load(fileName.c_str());
        if(!surf){
            SDL_Log("Failed to load surface: %s", fileName.c_str());
            return nullptr;
        }

        tex=SDL_CreateTextureFromSurface(mRenderer, surf);
        SDL_FreeSurface(surf);
        if(!tex){
            SDL_Log("Failed to convert surface to texture for: %s", fileName.c_str());
            return nullptr;
        }

        mTextures.emplace(fileName, tex);
    }
    return tex;
}

void Game::AddActor(Actor* actor){
    if(mUpdatingActors){
        mPendingActors.emplace_back(actor);
    }else{
        mActors.emplace_back(actor);
    }
}

void Game::RemoveActor(Actor* actor){
    auto iter=std::find(mPendingActors.begin(), mPendingActors.end(), actor);
    if(iter!=mPendingActors.end()){
        std::iter_swap(iter, mPendingActors.end()-1);
        mPendingActors.pop_back();
    }

    iter=std::find(mActors.begin(), mActors.end(), actor);
    if(iter!=mActors.end()){
        std::iter_swap(iter, mActors.end()-1);
        mActors.pop_back();
    }
}

void Game::AddSprite(SpriteComponent* sprite){
    int myDrawOrder=sprite->GetDrawOrder();
    auto iter=mSprites.begin();
    for(;iter!=mSprites.end();++iter){
        if(myDrawOrder<(*iter)->GetDrawOrder()){
            break;
        }
    }
    mSprites.insert(iter, sprite);
}

void Game::RemoveSprite(SpriteComponent* sprite){
    auto iter=std::find(mSprites.begin(), mSprites.end(), sprite);
    if(iter!=mSprites.end()){
        mSprites.erase(iter);
    }
}

void Game::AddAsteroid(Asteroid* ast){
    mAsteroids.emplace_back(ast);
}

void Game::RemoveAsteroid(Asteroid* ast){
    auto iter=std::find(mAsteroids.begin(), mAsteroids.end(), ast);
    if(iter!=mAsteroids.end()){
        mAsteroids.erase(iter);
    }
}