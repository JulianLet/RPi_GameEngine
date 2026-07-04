#pragma once

#include "Common.hpp"

constexpr bool USE_GRAVITY = true;
constexpr bool NO_GRAVITY = false;

enum class PhysicsType {STATIC = 0, KINEMATIC, DYNAMIC};

struct PhysicsComponent
{
    bool useGravity;
    PhysicsType physicsType;
    Vector2 currentVelocity;
    bool facingLeft;
}; 