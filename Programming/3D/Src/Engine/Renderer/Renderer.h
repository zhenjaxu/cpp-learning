#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <SDL2/SDL.h>

class Renderer
{
public:
    // Phong 反射
    struct DirectionalLight
    {
        Vector3 mDirection;     // 定向光
        Vector3 mDiffuseColor;  // 漫反射颜
        Vector3 mSpecColor;     // 镜面反射，specular
    };

    Renderer(class Game* mGame);
    ~Renderer();

    bool Initialize(float screenWidth, float screenHeight);
    void Draw();
    void Shutdown();
    void UnloadData();

    void AddSprite(class SpriteComponent* sprite);
    void RemoveSprite(class SpriteComponent* sprite);

    class Texture* GetTexture(const std::string& fileName);
    class Mesh* GetMesh(const std::string& fileName);

    void SetLightUniforms(Shader* shader);

private:
    bool LoadShaders();
    void CreateSpriteVerts();

    SDL_Window* mWindow;
    SDL_GLContext mContext;     // OpenGL上下文

    class VertexArray* mSpriteVerts;
    class Shader* mSpriteShader;
    class Shader* mMeshShader;
    class CameraActor* mCameraActor;    // 相机

    Matrix4 mView;
    Matrix4 mProjection;

    std::vector<class SpriteComponent*> mSprites;

    std::unordered_map<std::string, class Texture*> mTextures;
    std::unordered_map<std::string, class Texture*> mMeshes;
};