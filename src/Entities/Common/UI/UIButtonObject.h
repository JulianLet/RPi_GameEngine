#pragma once

#include "Entities/Entity.h"

#include "Common.h"
#include <cstdint>

class UIButtonObject : public Entity
{

    public:
    UIButtonObject(Vector2 pos, Vector2 size, uint16_t baseColor, uint16_t selectedColor, int layer);
    ~UIButtonObject() = default;
};