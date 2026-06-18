#include"Ship.h"
#include"../Engine/Core/Game.h"
#include"../Engine/Core/InputComponent.h"
#include"../Engine/Core/CircleComponent.h"
#include"../Engine/Renderer/AnimSpriteComponent.h"
#include"Laser.h"
#include"Asteroid.h"

Ship::Ship(Game* game)
:Actor(game)
,mLaserCooldown(0.0f)
{
    auto asc=new AnimSpriteComponent(this);
    std::vector<SDL_Texture*> anims={
        game->GetTexture("Assets/Textures/Ship/Ship01.png"),
        game->GetTexture("Assets/Textures/Ship/Ship02.png"),
        game->GetTexture("Assets/Textures/Ship/Ship03.png"),
        game->GetTexture("Assets/Textures/Ship/Ship04.png")
    };
    asc->SetAnimTextures(anims);

    auto ic=new InputComponent(this);
    ic->SetForwardKey(SDL_SCANCODE_W);
    ic->SetBackKey(SDL_SCANCODE_S);
    ic->SetClockwiseKey(SDL_SCANCODE_A);
    ic->SetCounterClockwiseKey(SDL_SCANCODE_D);
    ic->SetMaxForwardSpeed(300.0f);
    ic->SetMaxAngularSpeed(Math::TwoPi);

    mCircle=new CircleComponent(this);
    mCircle->SetRadius(30.0f);
}

void Ship::UpdateActor(float deltaTime){
    mLaserCooldown-=deltaTime;

    for(auto ast:GetGame()->GetAsteroids()){
        if(Intersect(*mCircle, *(ast->GetCircle()))){
            ast->SetState(EDead);
            break;
        }
    }
}

void Ship::ActorInput(const uint8_t* keyState){
    if(keyState[SDL_SCANCODE_SPACE]&&mLaserCooldown<=0.0f){
        auto laser=new Laser(GetGame());
        laser->SetPosition(GetPosition());
        laser->SetRotation(GetRotation());

        mLaserCooldown=0.5f;
    }
}
