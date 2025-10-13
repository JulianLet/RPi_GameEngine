#pragma once

#include "Entities/Component.h"
#include "Globals.h"
#include "Systems/Core/GameManager.h"

#include <functional>

struct UIButtonComponent : public Component
{
    bool isSelected = false;

    uint16_t defaultColor;
    uint16_t selectedColor;
    std::function<void(GameManager& manager)> OnClicked; //might be empty and just depach events

    UIButtonComponent(uint16_t defaultColor, uint16_t selectedColor) : defaultColor(defaultColor), selectedColor(selectedColor) {};
    ~UIButtonComponent() override;

    void Reset() override;
};