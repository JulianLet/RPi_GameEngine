#pragma once

#include "Entities/Component.h"
#include "Entities/Components/InputMapping.h"
#include "Hardware/Input.h"

#include <unordered_map>


struct InputIntendComponent : public Component
{
    int x = 0; //horizontal movement
    int y = 0; //vertical movement
    bool hasX = false;
    bool hasY = false;
    std::unordered_map<InputAction, bool> actions;

    InputIntendComponent() = default;
    ~InputIntendComponent() override;

    void Reset() override;
};