#pragma once
#include <SDL2/SDL.h>
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

    class Renderer* GetRenderer() const { return mRenderer; }

private:
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();
    void LoadData();
    void UnloadData();

    bool mUpdatingActors;
    std::vector<class Actor*> mActors;
    std::vector<class Actor*> mPendingActors;

    bool mIsRunning;
    Uint32 mTicksCount;

    class Renderer* mRenderer;
    class CameraActor* mCameraActor;
};