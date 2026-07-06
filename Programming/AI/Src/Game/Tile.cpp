#include"Tile.h"
#include"Game.h"
#include"SpriteComponent.h"
#include<string>

Tile::Tile(Game* game)
:Actor(game)
,mTileState(EDefault)
,mParent(nullptr)
,f(0.0f)
,g(0.0f)
,h(0.0f)
,mBlocked(false)
,mSelected(false)
,mInOpenSet(false)
,mInCloseSet(false)
{
    mSprite=new SpriteComponent(this);
    UpdateTexture();
}

void Tile::ToggleSelect(){
    mSelected=!mSelected;
    UpdateTexture();
}

void Tile::SetTileState(TileState state){
    mTileState=state;
    UpdateTexture();
}

void Tile::UpdateTexture(){
    std::string text;
    switch(mTileState){
        case EStart:
            text="Assets/Texture/TileTan.png";
            break;
        case EBase:
            text="Assets/Texture/TileGreen.png";
            break;
        case EPath:
            if(mSelected){
                text="Assets/Texture/TileGreySelected.png";
            }else{
                text="Assets/Texture/TileGrey.png";
            }
            break;
        case EDefault:
            if(mSelected){
                text="Assets/Texture/TileBrownSelected.png";
            }else{
                text="Assets/Texture/TileBrown.png";
            }
            break;
    }
}