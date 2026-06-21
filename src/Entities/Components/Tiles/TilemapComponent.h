#pragma once

#include <vector>


struct TilemapComponent
{
    int width;
    int height;

    uint16_t tilesetId;   // reference to global Tileset
    std::vector<uint8_t> tileIndices;
};

//on tilemap creation create entities with colliders in position