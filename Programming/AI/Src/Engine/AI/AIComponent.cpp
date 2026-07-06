#include"AIComponent.h"
#include"Actor.h"
#include"AIState.h"
#include<SDL2/SDL.h>

AIComponent::AIComponent(Actor* owner)
:Component(owner)
,mCurrentState(nullptr)
{}

void AIComponent::Update(float deltaTime){
    if(mCurrentState){
        mCurrentState->Update(deltaTime);
    }
}

void AIComponent::ChangeState(const std::string& name){
    if(mCurrentState){
        mCurrentState->OnExit();
    }
    
    auto iter=mStateMap.find(name);
    if(iter!=mStateMap.end()){
        mCurrentState=iter->second;
        mCurrentState->OnEnter();
    }
    else{
        SDL_Log("Could not find AIState %s in state map", name.c_str());
        return nullptr;
    }

}

void AIComponent::RegisterState(AIState* state){
    mStateMap.emplace(state->GetName(), state);
}