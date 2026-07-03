#pragma once

#include "Globals.h"
#include <cstdint>

struct SpriteDef
{
    const char* path;
    uint16_t width;
    uint16_t height;
    uint8_t indexOnSpritesheet;
};


struct SpriteDatabase
{
    SpriteDef sprites[MAX_SPRITES];
    bool filled[MAX_SPRITES] = {false};
};

struct Sprite
{
    uint16_t width;
    uint16_t height;
    uint16_t pixels[MAX_SPRITE_PIXELS];
};

struct SpriteCache
{
    Sprite sprites[MAX_SPRITES];
    bool loaded[MAX_SPRITES] = {false};
};