#pragma once
#include"Actor.h"

class Piece:public Actor{
public:
    Piece(class Game* game, float speed, float dropSpeed);

    void Update(float deltaTime) override;
    void ProcessInput(const uint8_t* keyState) override;
    void Draw(SDL_Renderer* renderer) override;

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