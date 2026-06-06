# 向量和基础物理

```cpp
#include"Math.h"
Vector2 myVector;
myVector.x=5;
myVector.y=10;
```

```cpp
Vector2 a, b;
Vector2 result=a-b;
```

```cpp
Vector2 a;
Vector2 result=5.0f*a;
```

```cpp
Vector2 a, b;
Vector2 result=a+b;
```

```cpp
Vector2 a;
float length=a.length();
```

```cpp
Vector2 a;
a.Normalize();
```
```cpp
Vector2 a;
Vector2 result=Vector2::Normalize(a);
```

```cpp
Vector3 Actor::GetForward() const {
    // Negate y-component for SDL
    return Vector2(Math::Cos(mRotation), -Math::Sin(mRotation));
}
```

```cpp
Vector2 shipToAsteroid=asteroid->GetPosition()-ship->GetPosition();

shipToAsteroid.Normalize();
float angle=Math::Atan2(-shipToAsteroid.y, shipToAsteroid.x);

ship->SetRotation(angle);
```

```cpp
float dotResult=Vector2::Dot(origForward, bewForward);
float angle=Math::Acos(dotResult);
```

```cpp
Vector3 c=Vector3::Cross(a, b);
```

```cpp
position+=GetForward()*forwardSpeed*deltaTime;

rotation+=angularSpeed*deltaTime;
```

```cpp
class MoveComponent:public Component{
public:
    MoveComponent(class Actor* owner, int updateOrder=10);
    
    void Update(float deltaTime) override;

    float GetAngularSpeed() const {return mAngularSpeed;}
    float GetForwardSpeed() const {return mForwardSpeed;}
    void SetAngularSpeed(float speed) {mAngularSpeed=speed;}
    void SetForwardSpeed(float speed) {mForwardSpeed=speed;}
    
private:
    float mAngularSpeed;
    float mForwardSpeed;
};
```

```cpp
void MoveComponent::Update(float deltaTime){
    if(!Math::NearZero(mAngularSpeed)){
        float rot=mOwner->GetRotation();
        rot+=mAngularSpeed*deltaTime;
        mOwner->SetRotation(rot);
    }
    if(!Math::NearZero(mForwardSpeed)){
        Vector2 pos=mOwner->GetPosition();
        pos+=mOwner->GetForward()*mForwardSpeed*deltaTime;
        mOwner->SetPosition(pos);
    }
}
```

```cpp
Asteroid::Asteroid(Game* game)
: Actor(game)
{
    Vector2 randPos=Random::GetVector(Vector2::Zero, Vector2(1024.0f, 768.0f));
    SetPosition(randPos);
    SetRotation(Random::GetFloatRange(0.0f, Math::TwoPi));

    SpriteComponent* sc=new SpriteComponent(this);
    sc->SetTexture(game->GetTexture("Assets/Asteroid.png"));
    
    MoveComponent* mc=new MoveComponent(this);
    mc->SetForwardSpeed(150.0f);
}
```

```cpp
const int numAsteroids=20;
for(int i=0;i<numAsteroids;++i){
    new Asteroid(this);
}
```

```cpp
virtual void ProcessInput(const uint8_t* keyState){}
```

```cpp
void ProcessInput(const uint8_t* keyState);
virtual void ActorInput(const uint8_t* keyState);
```
```cpp
void Actor::ProcessInput(const uint8_t* keyState){
    if(mState==EActive){
        for(auto comp:mComponents){
            comp->ProcessInput(keyState);
        }
        ActorInput(keyState);
    }
}
```
```cpp
mUpdatintActors=true;
for(auto actor:mActors){
    actor->ProcessInput(keyState);
}
mUpdatingActors=false;
```

```cpp
class InputComponent:public MoveComponent{
public:
    InputComponet(class Actor* owner);

    void ProcessInput(const uint8_t* keyState) override;

private:
    float mMaxForwardSpeed;
    float mMaxAngularSpeed;

    int mForwardKey;
    int mBackKey;

    int mClockwiseKey;
    int mCounterClockwiseKey;
};
```

```cpp
void InputComponent::ProcessInput(const uint8_t* keyState){
    float forwardSpeed=0.0f;
    if(keyState[mForwardKey]){
        forwardSpeed+=MaxForwardSpeed;
    }
    if(keyState[mBackKey]){
        forwardSpeed-=MaxForwardSpeed;
    }
    SetForwardSpeed(forwardSpeed);

    float angularSpeed=0.0f;
    if(keyState[mClockwiseKey]){
        angularSpeed+=mMaxAngularSpeed;
    }
    if(keyState[mCounterClockwiseKey]){
        angularSpeed-=mMaxAngularSpeed;
    }
    SetAngularSpeed(angularSpeed);
}
```