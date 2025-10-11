#pragma once

#include "Entities/Component.h"
#include "Globals.h"

#include <functional>

struct UIButtonComponent : public Component
{
    bool active;

    uint16_t defaultColor;
    uint16_t selectedColor;
    std::function<void(Entity* self, Entity* other)> OnClicked; //might be empty and just depach events

    UIButtonComponent(uint16_t defaultColor, uint16_t selectedColor) : defaultColor(defaultColor), selectedColor(selectedColor) {};
    ~UIButtonComponent() override;

    void Reset() override;
};