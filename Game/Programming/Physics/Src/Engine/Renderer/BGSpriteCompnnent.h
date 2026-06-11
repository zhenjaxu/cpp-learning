#pragma once
#include"SpriteComponent.h"
#include<vector>
#include"../Utils/Math.h"

class BGSpriteComponent:public SpriteComponent{
public:
    BGSpriteComponent(class Actor* owner, int drawOrder=10);

    void Update(float deltaTime) override;
    void Draw(SDL_Renderer* renderer) override;

    void SetBGTextures(const std::vector<SDL_Texture*>& textures;)

    void SetScreenSize(const Vector2& size){mScreenSize=size;}
    void SetScollSpeed(float speed){mScrollSpeed=speed;}
    float GetScollSpeed() const {return mScrollSpeed;}

private:
    struct BGTexture{
        SDL_Texture* mTexture;
        Vector2 mOffset;
    };

    std::vector<SDL_Texture*> mBGTextures;
    Vector2 mScreenSize;
    float mScrollSpeed;
};