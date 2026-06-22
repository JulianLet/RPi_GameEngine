#pragma once

#include "Common.hpp"
#include <unordered_set>
#include <unordered_map>

constexpr bool TRIGGER = true;
constexpr bool SOLID = false;

class Entity;

struct CollisionInfo
{
    float timeOfCollision;      //save collision and time of collision within last frame
    Vector2 collisionNormal;    //direction of collision
};

struct CollisionEntry
{
    uint32_t otherID = UINT32_MAX;
    CollisionInfo info;
};

constexpr int MAX_COLLISIONS = 5;

struct ColliderComponent
{
    Vector2 offset = {0,0};
    Vector2 size = {0,0};
    bool isTrigger;

    CollisionEntry currentCollisions[MAX_COLLISIONS];
};