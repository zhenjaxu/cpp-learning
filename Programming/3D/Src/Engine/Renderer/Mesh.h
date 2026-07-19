#pragma once
#include <string>
#include <vector>

class Mesh
{
public:
    Mesh();
    ~Mesh();

    bool Load(const std::string& fileName, class Game* game);
    void Unload();

    class VertexArray* GetVertexArray() { return mVertexArray; }
    class Texture* GetTexture(size_t index);
    const std::string& GetShaderName() const { return mShaderName; }
    float GetRadius() const { return mRadius; }
    
private:
    std::vector<class Texture*> mTextures;
    class VertexArray* mVertexArray;
    std::string mShaderName;
    float mRadius;
};