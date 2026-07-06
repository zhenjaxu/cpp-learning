#include"AnimSpriteComponent.h"

#include"../Utils/Math.h"

AnimSpriteComponent::AnimSpriteComponent(Actor* owner, int drawOrder, bool loop)
: SpriteComponent(owner, drawOrder),
  mCurrFrame(0.0f),
  mAnimFPS(24.0f),
  mLoop(loop)
{}

void AnimSpriteComponent::Update(float deltaTime){
    SpriteComponent::Update(deltaTime);

    if(mAnimTextures.size()>0){
        mCurrFrame+=mAnimFPS*deltaTime;
    }

    while(mCurrFrame>=mAnimTextures.size()){
        if(mLoop) mCurrFrame-=mAnimTextures.size();
        else mCurrFrame=mAnimTextures.size()-1;         // 播放完后停止
    }

    SetTexture(mAnimTextures[static_cast<int>(mCurrFrame)]);
}

void AnimSpriteComponent::SetAnimTextures(const std::vector<SDL_Texture*> textures){
    mAnimTextures=textures;
    if(mAnimTextures.size()>0){
        mCurrFrame=0.0f;
        SetTexture(mAnimTextures[0]);
    }
}