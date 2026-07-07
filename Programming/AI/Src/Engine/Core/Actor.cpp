#include"Actor.h"
#include"Game.h"
#include"Component.h"
#include<SDL2/SDL.h>
#include<algorithm>

Actor::Actor(Game* game)
:mGame(game)
,mState(EActive)
,mPosition(Vector2::Zero)
,mScale(1.0f)
,mRotation(0.0f)
{
    mGame->AddActor(this);
}

Actor::~Actor(){
    mGame->RemoveActor(this);

    while(!mComponents.empty()){
        delete mComponents.back();
    }
}

void Actor::Update(float deltaTime){
    if(mState==EActive){
        UpdateComponents(deltaTime);
        UpdateActor(deltaTime);
    }
}

void Actor::UpdateComponents(float deltaTime){
    for(auto comp:mComponents){
        comp->Update(deltaTime);
    }
}

void Actor::UpdateActor(float deltaTime){}

void Actor::ProcessInput(const uint8_t* keyState){
    if(mState==EActive){
        for(auto comp:mComponents){
            comp->ProcessInput(keyState);
        }

        ActorInput(keyState);
    }
}

void Actor::ActorInput(const uint8_t* keyState){}

void Actor::AddComponent(Component* component){
    int myUpdateOrder=component->GetUpdateOrder();
    auto iter=mComponents.begin();
    for(;iter!=mComponents.end();++iter){
        if(myUpdateOrder<(*iter)->GetUpdateOrder()){
            break;
        }
    }
    mComponents.insert(iter, component);
}

void Actor::RemoveComponent(Component* component){
    auto iter=std::find(mComponents.begin(), mComponents.end(), component);
    if(iter!=mComponents.end()) mComponents.erase(iter);
}