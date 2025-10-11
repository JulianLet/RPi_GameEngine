#pragma once

#include "Entities/Entity.h"
#include "Globals.h"

class Wall : public Entity
{
    uint16_t color = Color::BLACK;

    public:
    Wall(Vector2 pos, Vector2 size);
    ~Wall() = default;
};