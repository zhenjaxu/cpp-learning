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

Piece::Piece(Game* game, float speed, float dropSpeed)
:Actor(game)
,mPosition(Vector2{0,0})
,mType(0)
,mPrevA(false)
,mPrevD(false)
,mPrevW(false)
,mPrevSpace(false)
,mSpeed(speed)
,mDropSpeed(dropSpeed)
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
    
    CalculateGhost(mGhost);     // 提前计算幽灵块位置
}

void Piece::ProcessInput(const uint8_t* keyState){
    Vector2 nxt[4];
    Board* board=GetGame()->GetBoard();

    bool A=keyState[SDL_SCANCODE_A];
    bool D=keyState[SDL_SCANCODE_D];
    bool W=keyState[SDL_SCANCODE_W];
    bool Space=keyState[SDL_SCANCODE_SPACE];

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

    // 水平移动或旋转后一次，需重新计算幽灵块
    if((A&&!mPrevA)&&(D&&!mPrevD)&&!(W&&!mPrevW));
    else CalculateGhost(mGhost);     

    // 硬降，通过幽灵块直接获取降落位置
    if(Space&&!mPrevSpace){
        for(int i=0;i<4;++i) mBlocks[i]=mGhost[i];

        board->Lock(mBlocks, mType);
        board->ClearLines();
        Spawn();
    }

    mDropSpeed=keyState[SDL_SCANCODE_S]?0.05f:0.5f;
    mPrevA=A;
    mPrevD=D;
    mPrevW=W;
    mPrevSpace=Space;
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
    int cell=GetGame()->GetBoardCell();
    const SDL_Color& c=board->GetColors()[mType];
    SDL_Rect rc;

    for(int i=0;i<4;++i){
        // 绘制幽灵块
        if(mGhost[i].y<0) continue;     // 如果mGhost[i].y小于0，则mBlocks[i].y也会小于0
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);  // 混合模式，绘制时与底色混合
        SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 80);
        rc.x=mGhost[i].x*cell+1;
        rc.y=mGhost[i].y*cell+1;
        rc.w=cell-2;
        rc.h=cell-2;
        SDL_RenderFillRect(renderer, &rc);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);   // 恢复正常模式，绘制实体块

        if(mBlocks[i].y<0) continue;
        SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
        rc.x=mBlocks[i].x*cell+1;
        rc.y=mBlocks[i].y*cell+1;
        rc.w=cell-2;
        rc.h=cell-2;
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

// 计算下落到底的位置（幽灵块）
void Piece::CalculateGhost(Vector2 ghost[4]) const{
    for(int i=0;i<4;++i) ghost[i]=mBlocks[i];

    Board* board=GetGame()->GetBoard();
    while(true){
        Vector2 nxt[4];
        for(int i=0;i<4;++i) nxt[i]={ghost[i].x, ghost[i].y+1};
        if(!board->IsValid(nxt)) break;
        for(int i=0;i<4;++i) ghost[i]=nxt[i];
    }
}