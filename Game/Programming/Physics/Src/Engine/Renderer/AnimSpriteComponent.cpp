#include"AnimSpriteComponent.h"
#include"../Core/Actor.h"

AnimSpriteComponent::AnimSpriteComponent(Actor* owner, int drawOrder)
:SpriteComponent(owner, drawOrder)
,mCurrFrame(0.0f)
,mAnimFPS(24.0f)
{}

void AnimSpriteComponent::Update(float deltaTime){
    SpriteComponent::Update(deltaTime);

    if(!mAnimTextures.empty()){
        mCurrFrame+=mAnimFPS*deltaTime;

        while(mCurrFrame>=mAnimTextures.size()){
            mCurrFrame-=mAnimTextures.size();
        }

        SetTexture(mAnimTextures[static_cast<int>(mCurrFrame)]);
    }
}

void AnimSpriteComponent::SetAnimTextures(const std::vector<SDL_Texture*>& textures){
    mAnimTextures=textures;
    
    if(!mAnimTextures.empty()){
        mCurrFrame=0.0f;
        SetTexture(mAnimTextures[0]);
    }
}