#pragma once

#include <functional>

class Entity;

struct AIComponent
{
    std::function<void(World& world, uint8_t id, float)> think;
    std::function<void(World& world, uint8_t id, float)> decide;
    std::function<void(World& world, uint8_t id, float)> act;

    float memory[5];
};