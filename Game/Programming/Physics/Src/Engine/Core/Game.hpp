#pragma once
#include"SDL2/SDL.h"
#include<unordered_map>
#include<string>
#include<vector>

class Game{
public:
    Game();
    void Initialize();
    void RunLoop();
    void Shutdown();

    AddActor(class Actor* actor);
    RemoveActor(class Actor* actor);

    AddSprite(class SpriteComponent* sprite);
    RemoveSprite(class SpriteComponent* sprite);

    SDL_Texture* GetTexture(const std::string& fileName);

    AddAsteroid(class Asteroid* ast);
    RemoveAsteroid(class Asteroid* ast);

    std::vector<class Asteroid*> GetAsteroid(){return mAsteroids;}

private:
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();

    void LoadData();
    void UnloadData();

    std::unordered_map<std::string, SDL_Texture*> mTextures;

    std::vector<class Actor*> mActors;
    std::vector<class Actor*> mPendings;

    std::vector<class SpriteComponent*> mSprites;
    
    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;
    Uint32 mTicksCount;
    bool mIsRunning;

    bool mUpdatingActor;

    class Ship* mShip;
    std::vector<class Asteroid*> mAsteroids;
};