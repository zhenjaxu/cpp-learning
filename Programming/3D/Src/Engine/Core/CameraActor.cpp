#include "CameraActor.h"
#include "Game.h"
#include "MoveComponent.h"
#include "Renderer.h"
#include "Math.h"
#include <SDL2/SDL_scancode.h>

CameraActor::CameraActor(Game* game)
: Actor(game)
{
    mMoveComp = new MoveComponent(this);
}

void CameraActor::UpdateActor(float deltaTime)
{
    Actor::UpdateActor(deltaTime);

    Vector3 cameraPos = GetPosition();
    Vector3 target = GetPosition() + GetForward() * 100.0f;
    Vector3 up = Vector3::UnitZ;

    Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, up);
    GetGame()->GetRenderer()->SetViewMatrix(view);
}

void CameraActor::ActorInput(const uint8_t* keyState)
{
    float forwardSpeed = 0.0f;
    float angularSpeed = 0.0f;

    if(keyState[SDL_SCANCODE_W])
    {
        forwardSpeed += 300.0f;
    }

    if(keyState[SDL_SCANCODE_S])
    {
        forwardSpeed -= 300.0f;
    }

    if(keyState[SDL_SCANCODE_A])
    {
        angularSpeed -= Math::TwoPi;
    }

    if(keyState[SDL_SCANCODE_D])
    {
        angularSpeed += Math::TwoPi;
    }

    mMoveComp->SetForwardSpeed(forwardSpeed);
    mMoveComp->SetAngularSpeed(angularSpeed);
}