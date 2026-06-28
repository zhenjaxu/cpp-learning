#pragma once
#include"Actor.h"
#include<SDL2/SDL.h>
#include<vector>

struct Vector2{int x, y;};
extern const SDL_Color COLORS[8];

class Board:public Actor{
public:
    Board(class Game* game);

    void Draw(SDL_Renderer* renderer) override;

    bool IsValid(const Vector2 blocks[4]) const;
    void Lock(const Vector2 blocks[4], int type);
    void ClearLines();
    void Draw(SDL_Renderer* renderer) const;
    void Reset();

    void SetColumnsSize(int size){mColumnsSize=size;}
    void SetRowsSize(int size){mRowsSize=size;}
    void SetCellSize(float size){mCellSize=size;}
    int GetColumnsSize() const {return mColumnsSize;}
    int GetRowsSize() const {return mRowsSize;}
    float GetCellSize() const {return mCellSize;}

private:
    std::vector<std::vector<int>> mGrid;
    int mColumnsSize;
    int mRowsSize;
    float mCellSize;
};