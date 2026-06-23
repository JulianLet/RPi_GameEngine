#pragma once

#include "Globals.h"
#include "Managers/Game/GameManager.h"

#include <functional>

struct UIButtonComponent
{
    bool isSelected = false;

    uint8_t defaultColor;
    uint8_t selectedColor;
    std::function<void(GameManager& manager)> OnClicked; //might be empty and just depach events
};