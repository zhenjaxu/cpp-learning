#pragma once
#include"Actor.h"
#include<vector>

class Tile:public Actor{
public:
    friend class Grid;
    enum TileState{
        EDefault,
        EPath,
        EStart,
        EBase
    };

    Tile(class Game* game);

    void ToggleSelect();

    void SetTileState(TileState state);
    TileState GetTileState() const {return mTileState;}

    const Tile* GetParent() const {return mParent;}

private:
    void UpdateTexture();

    class SpriteComponent* mSprite;

    TileState mTileState;
    Tile* mParent;
    float f, g, h;
    bool mInOpenSet;
    bool mInClosedSet;
    bool mBlocked;
    bool mSelected;

    std::vector<Tile*> mAdjacent;
};