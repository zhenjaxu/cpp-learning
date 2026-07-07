#include"Grid.h"
#include"Game.h"
#include"Tile.h"
#include"Enemy.h"
#include"Tower.h"
#include<algorithm>

Grid::Grid(Game* game)
:Actor(game)
,mSelectedTile(nullptr)
{
    mTiles.resize(NumRows);
    for(size_t i=0;i<NumRows;++i){
        mTiles[i].resize(NumCols);
    }

    for(size_t i=0;i<NumRows;++i){
        for(size_t j=0;i<NumCols;++j){
            mTiles[i][j]=new Tile(GetGame());
            mTiles[i][j]->SetPosition(Vector2(TileSize/2.0f+j*TileSize, StartY+i*TileSize));
        }
    }

    GetStartTile()->SetTileState(Tile::EStart);
    GetEndTile()->SetTileState(Tile::EBase);

    for(size_t i=0;i<NumRows;++i){
        for(size_t j=0;i<NumCols;++j){
            if(i>0){
                mTiles[i][j]->mAdjacent.emplace_back(mTiles[i-1][j]);
            }
            if(i<NumRows-1){
                mTiles[i][j]->mAdjacent.emplace_back(mTiles[i+1][j]);
            }
            if(j>0){
                mTiles[i][j]->mAdjacent.emplace_back(mTiles[i][j-1]);
            }
            if(j<NumCols-1){
                mTiles[i][j]->mAdjacent.emplace_back(mTiles[i][j+1]);
            }
        }
    }

    FindPath(GetEndTile(), GetStartTile());  // 查找路径（反向）,可延mParent到达终点
    UpdatePathTile(GetStartTile());

    mNextEnemy=EnemyTime;
}

void Grid::UpdateActor(float deltaTime){
    Actor::UpdateActor(deltaTime);
    mNextEnemy-=deltaTime;
    if(mNextEnemy<=0.0f){
        new Enemy(GetGame());   // 自动添加进游戏
        mNextEnemy+=EnemyTime;
    }
}

void Grid::ProcessClick(int x, int y){
    y-=static_cast<int>(StartY-TileSize/2);
    if(y>=0){
        x/=static_cast<int>(TileSize);
        y/=static_cast<int>(TileSize);
        if(x>=0&&x<static_cast<int>(NumCols)&&y>=0&&y<static_cast<int>(NumRows)){
            SelectTile(y, x);
        }
    }
}

// A*算法搜索路径
void Grid::FindPath(Tile* start, Tile* goal){
    std::vector<Tile*> OpenSet;
    Tile* current=start;
    current->mInOpenSet=true;
    do{
        for(Tile* neighbor:current->mAdjacent){
            if(neighbor->mBlocked){
                continue;
            }

            if(!neighbor->mInClosedSet){
                if(!neighbor->mInOpenSet){
                    neighbor->mParent=current;
                    neighbor->h=(neighbor->GetPosition()-goal->GetPosition()).Length();
                    neighbor->g=current->g+TileSize;
                    neighbor->f=neighbor->g+neighbor->h;
                    openSet.emplace_back(neighbor);
                    neighbor->mInOpenSet=true;
                }
                else{
                    float NewG=current->g+TileSize;
                    if(NewG<neighbor->g){
                        neighbor->mParent=current;
                        neighbor->g=NewG;
                        neighbor->f=neighbor->g+neighbor->h;
                    }
                }
            }
        }
        
        if(openSet.empty()) break;

        // 选择成本与启发量之和最小的节点作为下一个节点
        auto iter=std::min_element(openSet.begin(), openSet.end(), 
            [](Tile* a, Tile* b){
                return a->f<b->f;
            }
        );
        current=*iter;
        openSet.erase(iter);
        current->mInOpenSet=false;
        current->mInClosedSet=true;
    }while(current!=goal);

    return current==goal;
}

void Grid::BuildTower(){
    if(mSelectedTile&&!mSelectedTile->mBlocked){
        mSelectedTile->mBlocked=true;
        if(FindPath(GetEndTile(), GetStartTile())){
            auto t=new Tower(GetGame());
            t->SetPosition(mSelectedTile->GetPosition()) 
        }else{
            mSelectedTile->mBlocked=false;
            FindPath(GetEndTile(), GetStartTile());
        }
        UpdatePathTiles(GetStartTile());
    }
}

void Grid::SelectTile(size_t row, size_t col){
    Tile::TileState tState=mTiles[row][col]->GetTileState();
    if(tState!=Tile::EStart&&Tile::EBase){
        // 取消先前的选择
        if(mSelectedTile){
            mSelectedTile->ToggleSelect();
        }

        mSelectedTile=mTiles[row][col];
        mSelectedTile->ToggleSelect();
    }
}

void Grid::UpdatePathTiles(Tile* start){
    for(size_t i=0;i<NumRows;++i){
        for(size_t j=0;j<NumCols;++j){
            if(!(i==3&&j==0)&&!(i==3&&j==15)){      // 排除起点和终点
                mTiles[i][j]->SetTileState(Tile::EDefault);
            }
        }
    }

    Tile* t=start->mParent;
    while(t!=GetEndTile()){
        t->SetTileState(Tile::EPath);
        t=t->mParent;
    }
}

Tile* Grid::GetStartTile(){
    return mTiles[3][0];
}

Tile* Grid::GetEndTile(){
    return mTiles[3][15];
}