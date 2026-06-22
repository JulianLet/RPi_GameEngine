#pragma once

#include "Common.hpp"

struct World;
class TransformComponent;

class PhysicsSystem
{
    public:
    PhysicsSystem() = default;
    ~PhysicsSystem() = default;

    void Update(World& world, float deltaTime);
    void ResolveCollisions(World& world, float deltaTime);
};