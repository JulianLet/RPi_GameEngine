#pragma once

#include "Common.h"
#include "Entities/Component.h"
#include <unordered_set>
#include <unordered_map>

#define TRIGGER true
#define SOLID false

class Entity;

struct ColliderComponent : public Component
{
    Vector2 offset = {0,0};
    Vector2 size = {0,0};
    bool isTrigger;

    std::unordered_map<Entity*, Vector2> currentCollisionsPos;

    ColliderComponent(Vector2 offset, Vector2 size, bool isTrigger) : offset(offset), size(size), isTrigger(isTrigger) {};
    ~ColliderComponent() override; 

    void Reset() override;
};