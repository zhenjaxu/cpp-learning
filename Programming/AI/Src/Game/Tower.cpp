#include"Tower.h"
#include"Game.h"
#include"Enemy.h"
#include"Bullet.h"
#include"SpriteComponent.h"
#include"MoveComponent.h"

Tower::Tower(Game* game)
:Actor(game)
,mNextAttack(AttackTime)
{
    auto sc=new SpriteComponent(this, 200);     // 绘制在最上层
    sc->SetTexture(game->GetTexture("Assets/Textures/Tower.png"));
}

void Tower::UpdateActor(float deltaTime){
    Actor::UpdateActor(deltaTime);

    mNextAttack-=deltaTime;
    Enemy* e=GetGame()->GetNearestEnemy(GetPosition());
    if(e){
        Vector2 dir=e->GetPosition()-GetPosition();
        float distSq=dir.LengthSq();
        if(distSq<AttackRange*AttackRange){
            SetRotation(Math::Atan2(-dir.y, dir.x));

            if(mNextAttack<=0.0f){
                auto b=new Bullet(GetGame());
                b->SetPosition(GetPosition());
                b->SetRotation(GetRotation());
                mNextAttack=AttackTime;
            }
        }
    }
}
