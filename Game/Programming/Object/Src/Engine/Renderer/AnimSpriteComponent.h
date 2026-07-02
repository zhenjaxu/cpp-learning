#pragma once
#include"SpriteComponent.h"
#include<vector>

class AnimSpriteComponent:public SpriteComponent{
public:
    AnimSpriteComponent(class Actor* owner, int drawOrder=100, bool loop=true);
    
    void Update(float deltaTime);
    
    void SetAnimTextures(const std::vector<SDL_Texture*> textures);

    float GetAnimFPS() const {return mAnimFPS;}
    void SetAnimFPS(float fps){mAnimFPS=fps;}

private:
    std::vector<SDL_Texture*> mAnimTextures;
    float mCurrFrame;
    float mAnimFPS;
    
    bool mLoop;     // 是否循环播放动画
};