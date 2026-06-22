#pragma once

#include "Common.hpp"

struct World;
struct TransformComponent;
struct ColliderComponent;

class CollisionSystem
{
public:
    CollisionSystem() = default;
    ~CollisionSystem() = default;
    
    void Update(World& world);
};