#include"Piece.h"
#include"Board.h"
#include"Game.h"
#include<cstdlib>
#include<ctime>

const Vector2 SHAPES[7][4]={
    {{0,-1},{0,0},{0,1},{0,2}},
    {{-1,0},{0,0},{1,0},{0,1}},
    {{0,0},{1,0},{0,1},{1,1}},
    {{0,-1},{0,0},{0,1},{1,1}},
    {{0,-1},{0,0},{0,1},{-1,1}},
    {{1,0},{0,0},{0,1},{-1,1}},
    {{-1,0},{0,0},{0,1},{1,1}}
};

Piece::Piece(Game* game)
:Actor(game)
,mPosition(Vector2{0,0})
,mType(0)
,mPrevA(false)
,mPrevD(false)
,mPrevW(false)
,mSpeed(0.0f)
,mDropSpeed(0.0f)
,mDropAccumulate(0.0f)
{
    srand((unsigned)time(nullptr));
    Spawn();
}

void Piece::Spawn(){
    mType=rand()%7;
    mPosition.x=GetGame()->GetBoardColumns()/2;
    mPosition.y=1;
    for(int i=0;i<4;++i){
        mBlocks[i].x=SHAPES[mType][i].x+mPosition.x;
        mBlocks[i].y=SHAPES[mType][i].y+mPosition.y;
    }
}

void Piece::ProcessInput(const uint8_t* keyState){
    Vector2 nxt[4];
    Board* board=GetGame()->GetBoard();

    bool A=keyState[SDL_SCANCODE_A];
    bool D=keyState[SDL_SCANCODE_D];
    bool W=keyState[SDL_SCANCODE_W];

    if(A&&!mPrevA){
        for(int i=0;i<4;++i) nxt[i]={mBlocks[i].x-1, mBlocks[i].y};
        if(board->IsValid(nxt)){
            mPosition.x--;
            for(int i=0;i<4;++i) mBlocks[i]=nxt[i];
        }
    }
    if(D&&!mPrevD){
        for(int i=0;i<4;++i) nxt[i]={mBlocks[i].x+1, mBlocks[i].y};
        if(board->IsValid(nxt)){
            mPosition.x++;
            for(int i=0;i<4;++i) mBlocks[i]=nxt[i];
        }
    }
    if(W&&!mPrevW){
        Rotate(nxt);
        if(board->IsValid(nxt)){
            for(int i=0;i<4;++i) mBlocks[i]=nxt[i];
        }
    }
    mDropSpeed=keyState[SDL_SCANCODE_S]?0.05f:0.5f;
    mPrevA=A;
    mPrevD=D;
    mPrevW=W;
}

void Piece::Update(float deltaTime){
    mDropAccumulate+=deltaTime;
    if(mDropAccumulate<mDropSpeed) return;

    mDropAccumulate-=mDropSpeed;
    Vector2 nxt[4];
    for(int i=0;i<4;++i) nxt[i]={mBlocks[i].x, mBlocks[i].y+1};

    Board* board=GetGame()->GetBoard();
    if(board->IsValid(nxt)){
        for(int i=0;i<4;++i) mBlocks[i]=nxt[i];
        mPosition.y++;
    }else{
        board->Lock(mBlocks, mType);
        board->ClearLines();
        Spawn();
        if(!board->IsValid(mBlocks)) {
            board->Reset();
            Spawn();
        }
    }
}

void Piece::Draw(SDL_Renderer* renderer){
    Board* board=GetGame()->GetBoard();
    for(int i=0;i<4;++i){
        if(mBlocks[i].y<0) continue;
        const SDL_Color& c=board->GetColors()[mType];
        SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
        int cell=GetGame()->GetBoardCell();
        SDL_Rect rc={
            mBlocks[i].x*cell+1,
            mBlocks[i].y*cell+1,
            cell-2,
            cell-2
        };
        SDL_RenderFillRect(renderer, &rc);
    }
}

void Piece::Rotate(Vector2 out[4]) const {
    for(int i=0;i<4;++i){
        int rx=mBlocks[i].x-mPosition.x;
        int ry=mBlocks[i].y-mPosition.y;
        out[i].x=mPosition.x+ry;
        out[i].y=mPosition.y-rx;
    }
}