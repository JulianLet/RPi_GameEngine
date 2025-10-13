#pragma once

#include "Entities/Component.h"

#include <functional>
#include <unordered_map>

class Entity;

struct AIComponent : public Component
{
    std::function<void(Entity*, float)> think;
    std::function<void(Entity*, float)> decide;
    std::function<void(Entity*, float)> act;

    std::unordered_map<int, float> memory;

    AIComponent() = default;
    ~AIComponent() override; 

    void Reset() override;
};