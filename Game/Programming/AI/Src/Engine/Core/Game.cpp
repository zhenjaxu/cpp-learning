#include"Game.h"
#include<SDL2/SDL_image.h>
#include<algorithm>
#include"Actor.h"
#include"SpriteComponent.h"
#include"Grid.h"
#include"Enemy.h"
#include"AIComponent.h"
#include"AIState.h"

Game::Game()
:mWindow(nullptr)
,mRenderer(nullptr)
,mIsRunning(true)
,mUpdatingActors(false)
{}

bool Game::Initialize(){
    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO)!=0){
        SDL_Log("Unable to initialize SDL: %s", GetError());
        return false;
    }

    mWindow=SDL_CreateWindow(
        "TowerDefense",
        200, 100,           // x, y
        1024, 768,          // w, h
        0
    );
    if(!mWindow){
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    mRenderer=SDL_CreateRenderer(
        mWindow,
        -1,
        SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC  // 图形加速、垂直同步
    );
    if(!mRenderer){
        SDL_Log("Failed to create Renderer: %s", SDL_GetError());
        return false;
    }

    if(!IMG_Init(IMG_INIT_PNG)){
        SDL_Log("Unable to initialize SDL_image: %s", SDL_GetError());
        return false;
    }

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
    UnloadDate();
    IMG_Quit();
    SDL_DestroyWindow(mWindow);
    SDL_DestroyRenderer(mRenderer);
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

    Uint8* keyState=SDL_GetKeyboard(NULL);
    if(keyState[SDL_SCANCODE_ESCAPE]){
        mIsRuninng=false;
    }

    if(keyState[SDL_SCANCODE_B]){
        mGrid->BuildTower();
    }

    int x, y;
    Uint32 buttons=SDL_GetMouseState(&x, &y);
    if(SDL_BUTTON(buttonds)&SDL_BUTTON_LEFT){   // 获取鼠标位置和左键信号
        mGrid->ProcessClick(x, y);
    }

    mUpdatingActors=true;
    for(auto actor:mActors){
        actor->ProcessInput(keyState);
    }
    mUpdatingActors=false;
}

void Game::UpdateGame(){
    Uin32 targetTicks=mTicksCount+16;
    if(SDL_TICKS_PASSED(targetTicks, SDL_GetTicks()+2)){
        SDL_Delay(targetTicks-SDL_GetTicks()-2);    // CPU sleep 到目标时间前1~2ms
    }

    while(!SDL_TICKS_PASSED(SDL_GetTicks(), targetTicks));  // 短时忙等
    float deltaTime=(SDL_GetTicks()-mTicksCount)/1000.0f;
    mTicksCount=SDL_GetTicks();

    if(deltaTime>0.05f) deltaTime=0.05f;    // 防止游戏跨度很大

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
    SDL_SetRenderDrawColor(mRenderer, 34, 139, 34, 255);
    SDL_RenderClear(mRenderer);

    for(auto sprite:mSprites){
        sprite->Draw(mRenderer);
    }

    SDL_RenderPresent(mRenderer);
}

void Game::LoadData(){
    mGrid=new Grid(this);
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
    }
    else{
        SDL_Surface* surf=IMG_Load(fileName.c_str());
        if(!surf){
            SDL_Log("Failed to load texture file %s", fileName.c_str());
            return nullptr;
        }

        tex=SDL_CreatTextureFromSurface(mRenderer, surf);
        if(!tex){
            SDL_Log("Failed to convert surface to texture for %s", fileName.c_str());
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
    if(iter!=mSprites.end()) mSprites.erase(iter);
}

Enemy* Game::GetNearestEnemy(const Vector2& pos){
    Enemy* best=nullptr;
    float bestDistSq=std::numeric_limits<float>::infinity();
    if(!mEnemies.empty()){
        for (size_t i=0; i<mEnemies.size(); ++i){
			float newDistSq = (pos - mEnemies[i]->GetPosition()).LengthSq();
			if (newDistSq < bestDistSq)
			{
				bestDistSq = newDistSq;
				best = mEnemies[i];
			}
		}
    }
    return best;
}