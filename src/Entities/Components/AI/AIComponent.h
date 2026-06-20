#pragma once

#include <functional>

class Entity;

struct AIComponent
{
    std::function<void(Entity*, float)> think;
    std::function<void(Entity*, float)> decide;
    std::function<void(Entity*, float)> act;

    float memory[5];
};