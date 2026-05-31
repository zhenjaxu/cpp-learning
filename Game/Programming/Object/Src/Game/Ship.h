#pragma once
#include"../Engine/Core/Actor.h"

class Ship:public Actor{
public: 
    Ship(class Game* game);

    void UpdateActor(float deltaTime);
    void ProcessKeyboard(const uint8_t* state);

    float GetRightSpeed() const {return mRightSpeed;}
    float GetDownSpeed() const {return mDownSpeed;}

private:
    float mRightSpeed;
    float mDownSpeed;
};