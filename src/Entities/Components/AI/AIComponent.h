#pragma once

#include <functional>

struct World;

struct AIComponent
{
    std::function<void(World&, uint8_t, float)> think;
    std::function<void(World&, uint8_t, float)> decide;
    std::function<void(World&, uint8_t, float)> act;

    float memory[5];
};