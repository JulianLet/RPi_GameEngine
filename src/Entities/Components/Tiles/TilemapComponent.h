#pragma once

#include "Systems/Resource/ResourceManager.h"
#include "Entities/Entity.h"

#include <vector>

class Renderer;

struct Tileset
{
    int tileWidth;
    int tileHeight;

    std::vector<std::vector<uint8_t>> tiles;
    std::vector<bool> solidTiles;
};

struct TilemapComponent
{
    int width;
    int height;

    uint16_t tilesetId;   // reference to global Tileset
    std::vector<uint8_t> tileIndices;
};

//on tilemap creation create entities with colliders in position