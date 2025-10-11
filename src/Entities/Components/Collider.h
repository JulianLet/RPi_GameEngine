#pragma once

#include "Common.h"
#include "Entities/Component.h"
#include <unordered_set>
#include <unordered_map>

#define STATIC true
#define NOT_STATIC  false
#define TRIGGER true
#define SOLID false

class Entity;

struct ColliderComponent : public Component
{
    Vector2 offset = {0,0};
    Vector2 size = {0,0};
    bool isTrigger;
    bool isStatic;

    std::unordered_map<Entity*, Vector2> currentCollisionsPos;

    ColliderComponent(Vector2 offset, Vector2 size, bool isStatic, bool isTrigger) : offset(offset), size(size), isStatic(isStatic), isTrigger(isTrigger) {};
    ~ColliderComponent() override; 

    void Reset() override;
};