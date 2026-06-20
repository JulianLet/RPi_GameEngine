#pragma once

#include "Entities/Component.h"
#include "Globals.h"
#include "Systems/Core/GameManager.h"

#include <functional>

struct UIButtonComponent
{
    bool isSelected = false;

    uint16_t defaultColor;
    uint16_t selectedColor;
    std::function<void(GameManager& manager)> OnClicked; //might be empty and just depach events
};