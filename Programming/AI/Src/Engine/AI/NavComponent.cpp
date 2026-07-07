#include"NavComponent.h"
#include"Actor.h"
#include"Tile.h"

NavComponent::NavComponent(Actor* owner)
:MoveComponent(owner)
,mNextNode(nullptr)
{}

void NavComponent::Update(float deltaTime){
    if(mNextNode){
        Vector2 diff=mOwner->GetPosition()-mNextNode->GetPosition();
        if(diff.LengthSq()<=2.0f*2.0f){
            mNextNode=mNextNode->GetParent();
            TurnTo(mNextNode->GetPosition());
        }
    }

    MoveComponent::Update(deltaTime);
}

void NavComponent::StartPath(const Tile* start){
    mNextNode=start->GetParent();
    TurnTo(mNextNode->GetPosition());
}

void NavComponent::TurnTo(const Vector2& pos){
    Vector2 dir=pos-mOwner->GetPosition();
    float angle=Math::Atan2(-dir.y, dir.x);
    mOwner->SetRotation(angle);
}