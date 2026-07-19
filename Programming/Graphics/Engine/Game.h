#pragma once
#include <SDL2/SDL.h>
#include <string>
#include <unordered_map>
#include <vector>
#include "Math.h"

class Game{
public:
    Game();
    bool Initialize();
    void RunLoop();
    void Shutdown();

    void AddActor(class Actor* actor);
    void RemoveActor(class Actor* actor);

    void AddSprite(class SpriteComponent* sprite);
    void RemoveSprite(class SpriteComponent* sprite);

    void AddAsteroid(class Asteroid* ast);
	void RemoveAsteroid(class Asteroid* ast);

    class Texture* GetTexture(const std::string& fileName);

private:
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();
    void LoadData();
    void UnloadData();

    void CreateSpriteVerts();
    bool LoadShaders();

    class VertexArray* mSpriteVerts;
    class Shader* mSpriteShader;

    std::vector<class SpriteComponent*> mSprites;
    std::unordered_map<std::string, class Texture*> mTextures;

    bool mUpdatingActors;

    std::vector<class Actor*> mActors;
    std::vector<class Actor*> mPendingActors;

    SDL_Window* mWindow;
    SDL_GLContext mContext;     // OpenGL上下文
    bool mIsRunning;
    Uint32 mTicksCount;

    class Ship* mShip;
	std::vector<class Asteroid*> mAsteroids;
};