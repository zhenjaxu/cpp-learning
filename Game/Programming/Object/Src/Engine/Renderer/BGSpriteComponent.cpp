#include"BGSpriteComponent.h"

#include"../Core/Actor.h"

BGSpriteComponent::BGSpriteComponent(Actor* owner, int drawOrder)
: SpriteComponent(owner, drawOrder),
  mScrollSpeed(0.0f)
{}

void BGSpriteComponent::Update(float deltaTime){
    SpriteComponent::Update(deltaTime);

    for(auto& bg:mBGTextures){
        bg.mOffset.x+=mScrollSpeed*deltaTime;

        if(bg.mOffset.x<-mScreenSize.x){
            bg.mOffset.x=mBGTextures.size()*mScreenSize.x+bg.mOffset.x;
        }
    }
}

void BGSpriteComponent::Draw(SDL_Renderer* renderer){
    for(auto& bg:mBGTextures){
        SDL_Rect r;

        r.w=static_cast<int>(mScreenSize.x);
        r.w=static_cast<int>(mScreenSize.y);
        r.x=static_cast<int>(mOwner->GetPosition().x-r.w/2+bg.mOffset.x);
        r.y=static_cast<int>(mOwner->GetPosition().y-r.h/2+bg.mOffset.y);

        SDL_RenderCopy(renderer, bg.mTexture, nullptr, &r);
    }
}