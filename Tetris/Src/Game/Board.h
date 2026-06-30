#pragma once
#include"Actor.h"
#include<SDL2/SDL.h>
#include<vector>

class Board:public Actor{
public:
    Board(class Game* game);

    void Init(int columns, int rows, int cell);
    void Draw(SDL_Renderer* renderer) override;

    bool IsValid(const Vector2 blocks[4]) const;
    void Lock(const Vector2 blocks[4], int type);
    void ClearLines();
    void Draw(SDL_Renderer* renderer) const;
    void Reset();

    int GetColumns() const {return mColumns;}
    int GetRows() const {return mRows;}
    int GetCell() const {return mCell;}

    const SDL_Color* GetColors() const {return mColors;}

private:
    std::vector<std::vector<int>> mGrid;
    int mColumns;
    int mRows;
    int mCell;

    const SDL_Color mColors[8];
};