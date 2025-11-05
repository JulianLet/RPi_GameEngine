#pragma once

#include "Common.hpp"
#include "Entities/Component.h"

#define USE_GRAVITY true
#define NO_GRAVITY false

enum class PhysicsType {STATIC = 0, KINEMATIC, DYNAMIC};

struct PhysicsComponent : public Component
{
    bool useGravity;
    PhysicsType physicsType;
    Vector2 startVelocity;
    Vector2 currentVelocity;

    PhysicsComponent(bool useGravity, Vector2 velocity, PhysicsType physicsType) : useGravity(useGravity), startVelocity(velocity), physicsType(physicsType) 
    {
        currentVelocity = startVelocity;
    };

    ~PhysicsComponent() override;

    void Reset() override;
}; 