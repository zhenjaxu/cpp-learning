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
    const Vector3& GetPosition() const {return mPosition;}
    float GetScale() const {return mScale;}
    float GetRotation() const {return mRotation;}

    void SetState(State state){mState=state;}
    void SetPosition(const Vector3& pos){mPosition=pos; mRecomputeWorldTransform=true;}
    void SetScale(float scale){mScale=scale; mRecomputeWorldTransform=true;}
    void SetRotation(float rotation){mRotation=rotation; mRecomputeWorldTransform=true;}

    Vector3 GetForward() const { return Vector3::Transform(Vector3::UintX, mRotation); }

    class Game* GetGame() { return mGame; }

private:
    State mState;
    Vector3 mPosition;      // 三维坐标
    float mScale;
    Quaternion mRotation;      // 四元数控制旋转

    Matrix4 mWorldTransform;
    bool mRecomputeWorldTransform;

    std::vector<class Component*> mComponents;
    class Game* mGame;
};