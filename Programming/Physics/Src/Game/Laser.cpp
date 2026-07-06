#include"Laser.h"
#include"../Engine/Core/Game.h"
#include"../Engine/Core/MoveComponent.h"
#include"../Engine/Core/CircleComponent.h"
#include"../Engine/Renderer/SpriteComponent.h"
#include"Asteroid.h"

Laser::Laser(Game* game)
:Actor(game)
,mDeathTimer(1.0f)
{
    auto sc=new SpriteComponent(this);
    sc->SetTexture(game->GetTexture("Assets/Textures/Laser.png"));

    auto mc=new MoveComponent(this);
    mc->SetForwardSpeed(800.0f);

    mCircle=new CircleComponent(this);
    mCircle->SetRadius(11.0f);
}

void Laser::UpdateActor(float deltaTime){
    mDeathTimer-=deltaTime;
    if(mDeathTimer<=0.0f){
        SetState(EDead);
    }

    else{
        for(auto ast:GetGame()->GetAsteroids()){
            if(Intersect(*mCircle, *(ast->GetCircle()))){
                SetState(EDead);
                ast->SetState(EDead);
                break;
            }
        }
    }
}