#pragma once

#include "Assets/AnimationDatabase.h"
#include "Assets/SpriteDatabase.h"
#include "Assets/Tileset.h"

struct Sprite;
struct World;

class AssetManager
{
public:
    SpriteDatabase mySprites;
    SpriteCache myCache;

    AnimationDatabase myAnimations;
    Tileset myTileset;

    AssetManager() = default;
    ~AssetManager() = default;

    uint8_t AddSprite(const char* path, uint16_t w, uint16_t h, uint8_t sheetIndex);
    Sprite& GetOrLoadSprite(int spriteID);
};