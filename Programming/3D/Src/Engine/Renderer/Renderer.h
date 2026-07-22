#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <SDL2/SDL.h>
#include "Math.h"

// Phong 反射
struct DirectionalLight
{
    Vector3 mDirection;     // 定向光
    Vector3 mDiffuseColor;  // 漫反射颜
    Vector3 mSpecColor;     // 镜面反射，specular
};

class Renderer
{
public:
    Renderer(class Game* game);
    ~Renderer();

    bool Initialize(float screenWidth, float screenHeight);
    void Shutdown();
    void UnloadData();

    void Draw();

    void AddSprite(class SpriteComponent* sprite);
    void RemoveSprite(class SpriteComponent* sprite);

    void AddMeshComp(class MeshComponent* mesh);
	void RemoveMeshComp(class MeshComponent* mesh);

    class Texture* GetTexture(const std::string& fileName);
    class Mesh* GetMesh(const std::string& fileName);

    void SetAmbientLight(const Vector3& ambient) { mAmbientLight = ambient; }
    void SetViewMatrix(const Matrix4& view) { mView = view; }

    DirectionalLight& GetDirectionalLight() { return mDirLight; }
    float GetScreenWidth() const { return mScreenWidth; }
	float GetScreenHeight() const { return mScreenHeight; }

private:
    bool LoadShaders();
    void CreateSpriteVerts();
    void SetLightUniforms(class Shader* shader);

    SDL_Window* mWindow;
    SDL_GLContext mContext;     // OpenGL上下文

    class Game* mGame;
    class VertexArray* mSpriteVerts;
    class Shader* mSpriteShader;
    class Shader* mMeshShader;

    Vector3 mAmbientLight;
	DirectionalLight mDirLight;

    Matrix4 mView;
    Matrix4 mProjection;

    float mScreenWidth;
	float mScreenHeight;

    std::vector<class SpriteComponent*> mSprites;
    std::vector<class MeshComponent*> mMeshComps;

    std::unordered_map<std::string, class Texture*> mTextures;
    std::unordered_map<std::string, class Mesh*> mMeshes;
};