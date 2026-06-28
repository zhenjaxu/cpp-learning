#pragma once
#include<cstdint>
#include<SDL2/SDL.h>

class Actor{
public:
    Actor(class Game* game);
    virtual ~Actor();

    virtual void Update(float deltaTime);
    virtual void ProcessInput(const uint8_t* keyState);
    virtual void Draw(SDL_Renderer* renderer);

    class Game* GetGame() const {return mGame;}

private:
    class Game* mGame;
};