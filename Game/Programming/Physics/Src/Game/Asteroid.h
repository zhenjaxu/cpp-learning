#pragma once
#include"../Engine/Core/Actor.h"

class Asteroid:public Actor{
public:
    Asteroid(class Game* game);
    ~Asteroid();

    class CircleComponent* GetCircle(){return mCircle;}

private:
    class CircleComponent* mCircle;
};