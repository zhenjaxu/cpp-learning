#include"Actor.h"
#include"Game.h"

Actor::Actor(Game* game)
:mGame(game)
,mState(EActive)
,mPosition(Vector2::Zero)
,mScale(1.0f)
,mRotation(0.0f)
,mRecomputeWorldTransform(true)
{
    game->AddActor();
}

Actor::~Actor(){}

Actor::Update(float deltaTime){
    if(mState==EActive){
        ComputeWorldTransform();

        UpdateComponents(deltaTime);
        UpdateActor(deltaTime);

        ComputeWorldTransform();
    }
}

Actor::ComputeWorldTransform(){
    if(mRecomputeWorldTransform){
        mRecomputeWorldTransform=false;

        mWorldTransform=Matrix4::CreateScale(mScale);
        mWorldTransform*=Matrix4::CreateRotationZ(mRotation);
        mWorldTransform*=Matrix4::CreateTranslation(Vector3(mPosition.x, mPosition.y, 0.0f));

        // 通知组件做出响应
        for(auto comp:mComponents){
            comp->OnUpdateWorldTransform();
        }
    }
}