#pragma once
#include"Math.h"

class Actor{
public:
    enum State{
        EActive,
        EDead,
        EPaused
    };

    Actor();
    virtual ~Actor();

    void Update(float deltaTime);
    void UpdateComponents(float deltaTime);
    virtual void UpdateActor(float deltaTime);

    void ComputeWorldTransform();

    void AddComponent(class Component* component);
    void RemoveComponent(class Component* component);

    State GetState() const {return mState;}
    Vector2 GetPosition() const {return mPosition;}
    float GetScale() const {return mScale;}
    float GetRotation() const {return mRotation;}

    void SetState(State state){mState=state;}
    void SetPosition(Vector2 pos){mPosition=pos;}
    void SetScale(float scale){mScale=scale;}
    void SetRotation(float rotation){mRotation=rotation;}

private:
    State mState;
    Vector2 mPosition;
    float mScale;
    float mRotation;

    Matrix4 mWorldTransform;            // 世界变换矩阵
    bool mRecomputeWorldTransform;      // 是否重新计算
};