#pragma once
#include <cstdint>

struct World;

struct CommonUIFactory
{
    uint8_t CreateUIButton(World& world, Vector2 pos, Vector2 size, uint8_t baseColor, uint8_t selectedColor, int layer);
    uint8_t CreateUIText(World& world, Vector2 position, const char* text, uint8_t textColor, int layer);
};