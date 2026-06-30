#pragma once

#include "Entities/Common/CommonFactory.h"
#include "Entities/Common/CommonUIFactory.h"
#include "Managers/Managers.h"

struct SDFactory
{
    CommonFactory myFactory;
    CommonUIFactory myUIFactory;

    uint8_t CreatePlayer(World& world, Vector2 startPos, Vector2 size, float moveSpeed, const char* path, int index);
    uint8_t CreateSpriteEntity(World& world, Vector2 pos, Vector2 size, const char* path, int index);
    uint8_t CreateTilemap(World& world);
};