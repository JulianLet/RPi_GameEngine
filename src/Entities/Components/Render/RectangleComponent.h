#pragma once
#include <cstdint>  // for uint16_t

constexpr bool FILLED = true;
constexpr bool OUTLINE = false;

struct RectangleComponent
{
    uint16_t currentColor = 0;
    bool filled = false;
}; 