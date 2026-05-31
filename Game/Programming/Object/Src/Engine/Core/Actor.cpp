#include"Actor.h"

#include<algorithm>

#include"Game.h"
#include"Component.h"

Actor::Actor(Game* game)
: mState(EActive),
  mPosition(Vector2::Zero),
  mScale(1.0f),
  mRotation(0.0f),
  mGame(game)
{
    mGame->AddActor(this);
}

