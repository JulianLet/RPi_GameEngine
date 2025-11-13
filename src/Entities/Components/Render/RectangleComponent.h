#pragma once
#include "Entities/Component.h"
#include <cstdint>  // for uint16_t

#define FILLED true
#define OUTLINE false

struct TransformComponent;

struct RectangleComponent : public Component
{
    TransformComponent* transform = nullptr;
    uint8_t startColor = 0;
    uint8_t currentColor = 0;
    bool filled = false;

    RectangleComponent(uint8_t color, bool filled) : startColor(color), filled(filled) 
    {
        currentColor = startColor;
    };
    ~RectangleComponent() override;

    void Reset() override;
}; 