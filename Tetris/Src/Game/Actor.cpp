#include"Actor.h"
#include"Game.h"

Actor::Actor(Game* game)
:mGame(game)
{
    mGame->AddActor(this);
}

Actor::~Actor(){
    mGame->RemoveActor(this);
}

void Actor::Update(float deltaTime){}

void Actor::ProcessInput(const uint8_t* keyState){}

void Actor::Draw(SDL_Renderer* renderer){}