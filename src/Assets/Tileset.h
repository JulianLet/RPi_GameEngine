#pragma once

#include <cstdint>
#include <vector>

constexpr int MAX_TILES = 15;

struct Tileset
{
    int tileWidth;
    int tileHeight;

    int sprites[MAX_TILES];
    bool solidTiles[MAX_TILES];
};