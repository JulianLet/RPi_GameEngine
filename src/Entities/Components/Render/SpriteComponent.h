#pragma once

#include "Entities/Component.h"

#include <vector>
#include <cstdint>

struct SpriteComponent : public Component
{
    int width;
    int height;
    std::vector<uint8_t> pixels;

    //spritesheet with flat binary
    SpriteComponent(int width, int height, const char *path, int index = 0);

    ~SpriteComponent() override;

    void Reset() override;
};