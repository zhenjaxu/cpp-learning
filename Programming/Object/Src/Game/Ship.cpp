#include"Ship.h"

#include"../Engine/Renderer/AnimSpriteComponent.h"
#include"../Engine/Core/Game.h"

Ship::Ship(Game* game)
: Actor(game),
  mRightSpeed(0),
  mDownSpeed(0)
{
    AnimSpriteComponent* asc=new AnimSpriteComponent(this);
    std::vector<SDL_Texture*> anim={
        game->GetTexture("Assets/Textures/Ship/Ship01.png"),
        game->GetTexture("Assets/Textures/Ship/Ship02.png"),
        game->GetTexture("Assets/Textures/Ship/Ship03.png"),
        game->GetTexture("Assets/Textures/Ship/Ship04.png")
    };
    asc->SetAnimTextures(anim);
}

void Ship::UpdateActor(float deltaTime){
    Actor::UpdateActor(deltaTime);

    Vector2 pos=GetPosition();
    pos.x+=mRightSpeed*deltaTime;
    pos.y+=mDownSpeed*deltaTime;

    if(pos.x<25.0f) pos.x=25.0f;
    if(pos.x>500.0f) pos.x=500.0f;
    if(pos.y<25.0f) pos.y=25.0f;
    if(pos.y>743.0f) pos.y=743.0f;

    SetPosition(pos);
}

void Ship::ProcessKeyboard(const uint8_t* state){
    mRightSpeed=0.0f;
    mDownSpeed=0.0f;
    if(state[SDL_SCANCODE_D]){
        mRightSpeed+=250.0f;
    }
    if(state[SDL_SCANCODE_A]){
        mRightSpeed-=250.0f;
    }
    if(state[SDL_SCANCODE_S]){
        mDownSpeed+=300.0f;
    }
    if(state[SDL_SCANCODE_W]){
        mDownSpeed-=300.0f;
    }
}