#pragma once

#include "Hardware/Input.h"

#include <unordered_map>

enum class InputAction {HORIZONTAL = 0, VERTICAL, START_GAME, JUMP, CLICK};

struct InputMappingComponent
{
    std::unordered_map<KEYCODE, std::pair<InputAction, int>> directionMapping;
    std::unordered_map<KEYCODE, InputAction> actionMapping;
};