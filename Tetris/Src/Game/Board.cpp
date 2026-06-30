#include"Board.h"
#include"Game.h"

Board::Board(Game* game)
:Actor(game)
,mColumns(0)
,mRows(0)
,mCell(0)
,mColors{
    {0,255,255,255}, {128,0,128,255}, {255,255,0,255},
    {255,165,0,255}, {0,0,255,255}, {0,255,0,255}, {255,0,0,255}
}
{}

void Board::Init(int columns, int rows, int cell){
    mColumns=columns;
    mRows=rows;
    mCell=cell;
    Reset();
}

void Board::Reset(){
    mGrid.assign(mRows, std::vector<int>(mColumns, -1));
}

bool Board::IsValid(const Vector2 blocks[4]) const {
    for(int i=0;i<4;++i){
        if(blocks[i].x<0||
           blocks[i].x>=mColumns||
           blocks[i].y>=mRows) return false;
        if(blocks[i].y>=0&&
           mGrid[blocks[i].y][blocks[i].x]!=-1) return false;
    }
    return true;
}

void Board::Lock(const Vector2 blocks[4], int type){
    for(int i=0;i<4;++i){
        if(blocks[i].y>=0) mGrid[blocks[i].y][blocks[i].x]=type;
    }
}

void Board::ClearLines(){
    for(int y=mRows-1;y>=0;--y){
        bool full=true;
        for(int x=0;x<mColumns;++x){
            if(mGrid[y][x]==-1){
                full=false;
                break;
            }
        }
        if(full){
            mGrid.erase(mGrid.begin()+y);
            mGrid.insert(mGrid.begin(), std::vector<int>(mColumns, -1));
            ++y;
        }
    }
}

void Board::Draw(SDL_Renderer* renderer){
    for(int y=0;y<mRows;++y){
        for(int x=0;x<mColumns;++x){
            if(mGrid[y][x]==-1) continue;
            const SDL_Color& c=mColors[mGrid[y][x]];
            SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
            SDL_Rect rc={x*mCell+1, y*mCell+1, mCell-2, mCell-2};
            SDL_RenderFillRect(renderer, &rc);
        }
    }
}