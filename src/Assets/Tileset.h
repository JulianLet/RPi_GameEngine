#pragma once

#include "Globals.h"
#include <cstdint>
#include <vector>


struct Tileset
{
    int tileWidth;
    int tileHeight;

    int sprites[MAX_TILES];
    bool solidTiles[MAX_TILES];
};