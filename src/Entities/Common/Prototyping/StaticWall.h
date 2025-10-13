#pragma once

#include "Entities/Entity.h"
#include "Globals.h"

#include <cstdint>

class StaticWall : public Entity
{
    public:
    StaticWall(Vector2 pos, Vector2 size, uint16_t color, const char* tag);
    ~StaticWall() = default;
};