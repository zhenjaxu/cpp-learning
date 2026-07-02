#include"Asteroid.h"
#include"../Engine/Core/Game.h"
#include"../Engine/Core/MoveComponent.h"
#include"../Engine/Core/CircleComponent.h"
#include"../Engine/Renderer/SpriteComponent.h"
#include"../Engine/Utils/Random.h"

Asteroid::Asteroid(Game* game)
:Actor(game)
{
    SetPosition(Random::GetVector(Vector2::Zero, Vector2(1024.0f, 768.0f)));
    SetRotation(Random::GetFloatRange(0.0f, Math::TwoPi));

    auto sc=new SpriteComponent(this);
    sc->SetTexture(game->GetTexture("Assets/Textures/Asteroid.png"));
    
    auto mc=new MoveComponent(this);
    mc->SetForwardSpeed(150.0f);

    mCircle=new CircleComponent(this);
    mCircle->SetRadius(40.0f);

    game->AddAsteroid(this);
}

Asteroid::~Asteroid(){
    GetGame()->RemoveAsteroid(this);
}