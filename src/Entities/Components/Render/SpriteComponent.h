#pragma once
#include <cstdint>

struct SpriteComponent
{
    uint16_t frame = 0;       // final frame to render
    uint16_t frameIndex = 0;  // local animation frame
};