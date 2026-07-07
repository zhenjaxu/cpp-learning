#pragma once
#include<SDL2/SDL.h>
#include<vector>
#include<unordered_map>
#include<string>
#include"Math.h"

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

    SDL_Texture* GetTexture(const std::string& fileName);

    class Grid* GetGrid() const {return mGrid;}
    std::vector<class Enemy*>& GetEnemies() {return mEnemies;}

    class Enemy* GetNearestEnemy(const Vector2& pos);

private:
    // 游戏循环
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();

    // 加载和释放资源
    void LoadData();
    void UnloadData();

    // 游戏对象
    std::vector<class Actor*> mActors;
    std::vector<class Actor*> mPendingActors;   // 正在更新时，进入等待队列
    
    bool mUpdatingActors;
    
    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;
    Uint32 mTicksCount;
    bool mIsRunning;

    std::vector<class SpriteComponent*> mSprites;   // 动画组件

    std::unordered_map<std::string, SDL_Texture*> mTextures;    // 图形库

    std::vector<class Enemy*> mEnemies;
    class Grid* mGrid;

    float mNextEnemy;
};