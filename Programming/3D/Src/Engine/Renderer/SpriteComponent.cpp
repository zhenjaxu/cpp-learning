#include "SpriteComponent.h"
#include "Shader.h"
#include "Actor.h"
#include "Game.h"
#include "Texture.h"

SpriteComponent::SpriteComponent(Actor* owner, int drawOrder)
:Component(owner)
,mTexture(nullptr)
,mDrawOrder(drawOrder)
,mTexWidth(0)
,mTexHeight(0)
{
	mOwner->GetGame()->AddSprite(this);
}

SpriteComponent::~SpriteComponent()
{
	mOwner->GetGame()->RemoveSprite(this);
}


void SpriteComponent::Draw(Shader* shader){
    Matrix4 scaleMat=Matrix4::CreateScale(
        static_cast<float>(mTexWidth),
        static_cast<float>(mTexHeight),
        1.0f
    );

    Matrix4 world=scaleMat*mOwner->GetWorldTransform();
    shader->SetMatrixUniform("uWorldTransform", world);

    mTexture->SetActive();

    glDrawElements(
        GL_TRIANGLES,       // 绘制形状，三角形
        6,                  // 索引缓冲区中的索引数量
        GL_UNSIGNED_INT,    // 索引类型
        nullptr             // 通常是nullptr
    );
}

void SpriteComponent::SetTexture(Texture* texture)
{
	mTexture = texture;
	mTexWidth = texture->GetWidth();
	mTexHeight = texture->GetHeight();
}