#include "MeshComponent.h"
#include "Shader.h"
#include "Actor.h"
#include "Math.h"
#include "Texture.h"
#include "VertexArray.h"
#include <GL/glew.h>

void MeshComponent::Draw(Shader* shader)
{
    if(Mesh)
    {
        shader->SetMatrixUniform("uWorldTransform", mOwner->GetWorldTransform());
        Texture* t = mMesh->GetTexture(mTextureIndex);
        VertexArray* va = mMesh->GetVertexArray();
        va->SetActive();
        glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
    }
}