#pragma once
#include"Actor.h"

class Piece:public Actor{
public:
    Piece(class Game* game);

    void Update(float deltaTime) override;
    void ProcessInput(const uint8_t* keyState) override;
    void Draw(SDL_Renderer* renderer) override;

    void SetSpeed(float speed){mSpeed=speed;}
    void SetDropSpeed(float speed){mDropSpeed=speed;}

private:
    void Spawn();
    void Rotate(Vector2 out[4]) const;

    Vector2 mBlocks[4];
    Vector2 mPosition;
    int mType;

    bool mPrevA;
    bool mPrevD;
    bool mPrevW;
    
    float mSpeed;
    float mDropSpeed;
    float mDropAccumulate;
};