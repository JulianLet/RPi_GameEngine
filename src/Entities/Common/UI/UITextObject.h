#pragma once

#include "Entities/Entity.h"

#include <cstdint>  // for uint16_t

class UITextObject : public Entity
{

    public:
    UITextObject(Vector2 position, const char* text, uint16_t textColor, int layer);
    ~UITextObject() = default;
};
