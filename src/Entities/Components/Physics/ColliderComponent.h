#pragma once

#include "Common.hpp"
#include "Entities/Component.h"
#include <unordered_set>
#include <unordered_map>

#define TRIGGER true
#define SOLID false

class Entity;

struct CollisionInfo
{
    float timeOfCollision;      //save collision and time of collision within last frame
    Vector2 collisionNormal;    //direction of collision
};

struct ColliderComponent : public Component
{
    Vector2 offset = {0,0};
    Vector2 size = {0,0};
    bool isTrigger;

    std::unordered_map<Entity*, CollisionInfo> currentCollisions;

    ColliderComponent(Vector2 offset, Vector2 size, bool isTrigger) : offset(offset), size(size), isTrigger(isTrigger) {};
    ~ColliderComponent() override; 

    void Reset() override;
};