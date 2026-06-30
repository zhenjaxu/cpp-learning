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

    class Board* GetBoard() const {return mBoard;}
    int GetBoardColumns() const {return mBoardColumns;}
    int GetBoardCell() const {return mBoardCell;}

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

    const int mBoardColumns;
    const int mBoardRows;
    const int mBoardCell;

    std::vector<class Actor*> mActors;

    class Board* mBoard;
    class Piece* mPiece;
};