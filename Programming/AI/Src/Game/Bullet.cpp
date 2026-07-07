#include"Bullet.h"
#include"Game.h"
#include"Enemy.h"
#include"SpriteComponent.h"
#include"MoveComponent.h"
#include"CircleComponent.h"

Bullet::Bullet(class Game* game)
:Actor(game)
,mLiveTime(1.0f)
{
    auto sc=new SpriteComponent(this);
    sc->SetTexture(game->GetTexture("Assets/Textures/Projectile.png"));

    auto mc=new MoveComponent(this);
    mc->SetForwardSpeed(400.0f);
    
    mCircle=new CircleComponent(this);
    mCircle->SetRadius(5.0f);
}

void Bullet::UpdateActor(float deltaTime){
    Actor::UpdateActor(deltaTime);

    mLiveTime-=deltaTime;
    if(mLiveTime<=0.0f){
        SetState(EDead);
        return;
    }
    
    for(auto enemy:GetGame()->GetEnemies()){
        if(Intersect(*mCircle, *(enemy->GetCircle()))){
            enemy->SetState(EDead);
            SetState(EDead);
            break;
        }
    } 
}