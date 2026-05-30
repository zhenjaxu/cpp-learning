#pragma once
#include"SpriteComponent.h"
#include<vector>

class AnimSpriteComponent:public SpriteComponent{
public:
    AnimSpriteComponent(class Actor* owner, int drawOrder=100);
    
    void Update(float deltaTime);
    
    void SetTextures(const std::vector<SDL_Texture*> textures);

    float GetAnimFPS() const {return mAnimFPS;}
    void SetAnimFPS(float fps){mAnimFPS=fps;}

private:
    std::vector<SDL_Texture*> mTextures;
    float mCurrFrame;
    float mAnimFPS;
};