#pragma once

#include "Entities/Entity.h"
#include "Globals.h"

class Plattform : public Entity
{
private:
    uint16_t color = Color::BLACK;

    public:
    Plattform(Vector2 pos, Vector2 size);
    ~Plattform() = default;
};