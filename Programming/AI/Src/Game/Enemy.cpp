#include"Enemy.h"
#include"Game.h"
#include"Grid.h"
#include"Tile.h"
#include"Math.h"
#include"CircleComponent.h"
#include"NavComponent.h"
#include"SpriteComponent.h"
#include<algorithm>

Enemy::Enemy(Game* game)
:Actor(game)
{
    game->GetEnemies().emplace_back(this);

    auto sc=new SpriteComponent(this);
    sc->SetTexture(game->GetTexture("Assets/Textures/Airplane.png"));

    Tile* start=game->GetGrid()->GetStartTile();
    SetPosition(start->GetPosition());

    auto nc=new NavComponent(this);
    nc->SetForwardSpeed(150.0f);
    nc->StartPath(start);

    mCircle=new CircleComponent(this);
    mCircle->SetRadius(25.0f);
}

Enemy::~Enemy(){
    auto iter=std::find(GetGame()->GetEnemies().begin(),
                        GetGame()->GetEnemies().end(),
                        this);
    if(iter!=GetGame()->GetEnemies().end()) GetGame()->GetEnemies().erase(iter);                   
}

void  Enemy::UpdateActor(float deltaTime){
    Actor::UpdateActor(deltaTime);

    // 到终点销毁
    Vector2 diff=GetPosition()-GetGame()->GetGrid()->GetEndTile()->GetPosition();
    if(diff.LengthSq()<10.0f*10.0f){
        SetState(EDead);
    }
}