#pragma once

#include <cstdint>
#include <vector>

struct Tileset
{
    int tileWidth;
    int tileHeight;

    std::vector<std::vector<uint8_t>> tiles;
    std::vector<bool> solidTiles;
};