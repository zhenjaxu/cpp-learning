#pragma once
#include"Board.h"

class Piece{
public:
    Vector2 blocks[4];
    int cx, cy;
    int type;

    void Spawn();
    void Rotate(Vector2 out[4]) const;
};