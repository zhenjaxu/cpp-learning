#pragma once
#include"SpriteComponent.h"
#include<vector>
#include"../utils/Math.h"

class BGSpriteComponent:public SpriteComponent{
public:
    BGSpriteComponent(class Actor* owner, int drawOrder=10);

    void Update(float deltaTime) override;
    void Draw(SDL_Renderer* renderer) override;

    void SetBGTextures(const std::vector<SDL_Texture*>& textures);
    
    void SetScreenSize(const Vector2& size){mScreenSize=size;}
    void SetScrollSpeed(float speed){mScrollSpeed=speed;}
    float GetScrollSpeed() const {return mScrollSpeed;}

private:
    struct BGTexture{
        SDL_Texture* texture;
        Vector2 mOffset;
    };
    std::vector<BGTexture> BGTextures;
    Vector2 mScreenSize;
    float mScrollSpeed;
};