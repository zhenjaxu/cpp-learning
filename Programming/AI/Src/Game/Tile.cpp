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
,mInClosedSet(false)
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
            text="Assets/Textures/TileTan.png";
            break;
        case EBase:
            text="Assets/Textures/TileGreen.png";
            break;
        case EPath:
            if(mSelected){
                text="Assets/Textures/TileGreySelected.png";
            }else{
                text="Assets/Textures/TileGrey.png";
            }
            break;
        case EDefault:
            if(mSelected){
                text="Assets/Textures/TileBrownSelected.png";
            }else{
                text="Assets/Textures/TileBrown.png";
            }
            break;
    }
    mSprite->SetTexture(GetGame()->GetTexture(text));
}