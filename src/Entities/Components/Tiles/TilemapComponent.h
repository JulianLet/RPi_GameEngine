#pragma once

#include "Systems/Resource/ResourceManager.h"
#include "Entities/Component.h"
#include "Entities/Entity.h"

#include <vector>

class Renderer;

struct Tileset
{
    int tileWidth;
    int tileHeight;
    File spritesheet;
    std::vector<bool> solidTiles;

    Tileset(int w, int h, int tileCount, const char* filePath);
    ~Tileset();

    bool IsSolid(int index);
    std::vector<uint8_t> GetSprite(int index);
};

struct TilemapComponent : public Component
{
    int width;
    int height;
    Tileset tileset;
    std::vector<uint8_t> tileIndices;

    TilemapComponent(int width, int height, const char* filePath, Tileset tileset);
    ~TilemapComponent();

    void Render(Renderer& renderer, int screenX, int screenY, float zoom);
    void Reset() override;
};

//on tilemap creation create entities with colliders in position