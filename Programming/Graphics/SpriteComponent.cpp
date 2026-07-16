#include"SpriteComponent.h"
#include"Shader.h"

void SpriteComponent::Draw(Shader* shader){
    Matrix4 scaleMat=Matrix4::CreateScale(
        static_cast<float>(mTexWidth),
        static_cast<float>(mTexHeight),
        1.0f
    );

    Matrix4 world=scaleMat*mOwner->GetWorldTransform();
    shader->SetMatrixUniform("uWorldTransform", world);

    glDrawElements(
        GL_TRIANGLES,       // 绘制形状，三角形
        6,                  // 索引缓冲区中的索引数量
        GL_UNSIGHED_INT,    // 索引类型
        nullptr             // 通常是nullptr
    );
}

