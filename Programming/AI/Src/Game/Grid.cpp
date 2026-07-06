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

void Grid::UpdateActor(float deltaTime){}

void Grid::ProcessClick(int x, int y){}

void Grid::FindPath(Tile* start, Tile* goal){}

void Grid::BuildTower(){}

Tile* Grid::GetStartTile(){}

Tile* Grid::GetEndTile(){}

void Grid::SelectTile(size_t row, size_t col){}

void Grid::UpdatePathTiles(Tile* start){}