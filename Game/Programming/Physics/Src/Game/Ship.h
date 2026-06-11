#pragma once
#include"../Engine/Core/Actor.h"
#include<cstdint>

class Ship:public Actor{
public:
    Ship(class Game* game);

    void UpdateActor(float deltaTime) override;
    void ActorInput(const uint8_t* keyState) override;

private:
    float mLaserCooldown;
};