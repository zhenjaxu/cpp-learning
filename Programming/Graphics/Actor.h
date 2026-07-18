#pragma once
#include "Math.h"
#include <vector>
#include <cstdint>

class Actor{
public:
    enum State{
        EActive,
        EDead,
        EPaused
    };

    Actor(class Game* game);
    virtual ~Actor();

    void Update(float deltaTime);
    void UpdateComponents(float deltaTime);
    virtual void UpdateActor(float deltaTime);

    void ProcessInput(const uint8_t* keyState);
    virtual void ActorInput(const uint8_t* keyState);

    void ComputeWorldTransform();
    const Matrix4& GetWorldTransform() const { return mWorldTransform; }

    void AddComponent(class Component* component);
    void RemoveComponent(class Component* component);

    State GetState() const {return mState;}
    const Vector2& GetPosition() const {return mPosition;}
    float GetScale() const {return mScale;}
    float GetRotation() const {return mRotation;}

    void SetState(State state){mState=state;}
    void SetPosition(const Vector2& pos){mPosition=pos; mRecomputeWorldTransform=true;}
    void SetScale(float scale){mScale=scale; mRecomputeWorldTransform=true;}
    void SetRotation(float rotation){mRotation=rotation; mRecomputeWorldTransform=true;}

    class Game* GetGame() { return mGame; }
    
    void AddComponent(class Component* component);
    void RemoveComponent(class Component* component);

private:
    State mState;
    Vector2 mPosition;
    float mScale;
    float mRotation;

    Matrix4 mWorldTransform;            // 世界变换矩阵
    bool mRecomputeWorldTransform;      // 是否重新计算

    std::vector<class Component*> mComponents;
    class Game* mGame;
};