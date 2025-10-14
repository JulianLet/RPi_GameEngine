#pragma once

#include "Entities/Entity.h"

#include <cstdint>  // for uint16_t

class UITimerText : public Entity
{

    public:
    UITimerText(Vector2 position, uint16_t textColor, int layer);
    ~UITimerText() = default;
};
