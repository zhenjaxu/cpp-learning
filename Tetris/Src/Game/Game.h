#pragma once
#include<SDL2/SDL.h>
#include<vector>

class Game{
public:
    Game();
    bool Initialize();
    void RunLoop();
    void Shutdown();

    void AddActor(class Actor* actor);
    void RemoveActor(class Actor* actor);

private:
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();

    void LoadData();
    void UnloadData();

    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;
    Uint32 mTicksCount;
    bool mIsRunning;

    int mBoardColumns;
    int mBoardRows;
    float mBoardCell;

    std::vector<class Actor*> mActors;

    class Board* mBoard;
    class Piece* mPiece;
};