#pragma once

#include "Entities/Entity.h"

#include <cstdint>  // for uint16_t

class TextObject : public Entity
{

    public:
    TextObject(Vector2 position, const char* text, uint16_t textColor);
    ~TextObject() = default;
};
