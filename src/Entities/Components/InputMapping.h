#pragma once

#include "Entities/Component.h"
#include "Hardware/Input.h"

#include <unordered_map>

enum class InputAction {HORIZONTAL = 0, VERTICAL, START_GAME, JUMP, CLICK};

struct InputMappingComponent : public Component
{
    std::unordered_map<KEYCODE, std::pair<InputAction, int>> directionMapping;
    std::unordered_map<KEYCODE, InputAction> actionMapping;

    InputMappingComponent() = default;
    ~InputMappingComponent() override;

    void Reset() override;
};