#pragma once
#include <string>

class Renderer
{
public:
    Renderer();
    ~Renderer();

    bool Initialize(float screenWidth, float screenHeight);
    void Draw();
    void Shutdown();
    void UnloadData();

    void AddSprite(class SpriteComponent* sprite);
    void RemoveSprite(class SpriteComponent* sprite);

    class Texture* GetTexture(const std::string& fileName);
    class Mesh* GetMesh(const std::string& fileName);

private:
    bool LoadShaders();
    void CreateSpriteVerts();

    
};