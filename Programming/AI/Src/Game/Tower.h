#pragma once
#include"Actor.h"

class Tower:public Actor{
public:
    Tower(class Game* game);
    void UpdateActor(float deltaTime) override;

private:
    float mNextAttack;
    const float AttackTime=3.0f;
    const float AttackRange=100.0f;
};