#pragma once

#include "Entities/Component.h"

#include <vector>
#include <cstdint>

struct SpriteComponent : public Component
{
    int width;
    int height;
    std::vector<uint16_t> pixels;

    //whole file is one sprite
    SpriteComponent(int width, int height, const char* path); 

    //spritesheet with flat binary
    SpriteComponent(int width, int height, const char *path, int index);

    //spritesheet with rows?!?! lets see when i get there
    SpriteComponent(int width, int height, const char* path, int posX, int posY, int sheetWidth); 
    ~SpriteComponent() override;

    void Reset() override;
};