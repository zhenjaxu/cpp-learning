#pragma once
#include"MoveComponent.hpp"
#include<cstdint>

class InputComponent:public MoveComponent{
public:
    InputComponent(class Actor* owner);

    void ProcessInput(const uint8_t* keyState) override;

    float GetMaxForward() const {return mMaxForward;}
    float GetMaxAngular() const {return mMaxAngular;}
    
    void SetMaxForward(float speed){mMaxForward=speed;}
    void SetMaxAngular(float speed){mMaxAngular=speed;}

    int GetForwardKey() const {return mForwardKey;}
    int GetAngularKey() const {return mAngularKey;}
    int GetClockwiseKey() const {return mClockwiseKey;}
    int GetCounterClockwiseKey() const {return mCounterClockwiseKey;}

    void SetForwardKey(int key){mForwardKey=key;}
    void SetAngularKey(int key){mAngularKey=key;}
    void SetClockwiseKey(int key){mClockwiseKey=key;}
    void SetCounterClockwiseKey(int key){mCounterClockwiseKey=key;}

private:
    float mMaxForward;
    float mMaxAngular;
    
    int mForwardKey;
    int mAngularKey;
    int mClockwiseKey;
    int mCounterClockwiseKey;
};