#pragma once
#include <cstdint>

struct SpriteDef
{
    const char* path;
    uint16_t width;
    uint16_t height;
    uint8_t indexOnSpritesheet;
};

constexpr int MAX_SPRITES = 32;

struct SpriteDatabase
{
    SpriteDef sprites[MAX_SPRITES];
};

struct Sprite
{
    uint16_t width;
    uint16_t height;
    uint16_t* pixels;
};

struct SpriteCache
{
    Sprite sprites[MAX_SPRITES];
};