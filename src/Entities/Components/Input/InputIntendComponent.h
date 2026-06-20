#pragma once

#include "InputMappingComponent.h"
#include "Hardware/Input.h"

#include <unordered_map>


struct InputIntendComponent
{
    int x = 0; //horizontal movement
    int y = 0; //vertical movement
    bool hasX = false;
    bool hasY = false;
    std::unordered_map<InputAction, bool> actions;
};