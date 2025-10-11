#pragma once

#include "Common.h"
#include "Entities/Component.h"

#define USE_GRAVITY true
#define NO_GRAVITY false
#define KINEMATIC true
#define DYNAMIC false

struct PhysicsComponent : public Component
{
    bool useGravity;
    bool isKinematic = false; // moves only by input/AI
    Vector2 startVelocity;
    Vector2 currentVelocity;

    PhysicsComponent(bool useGravity, Vector2 velocity, bool isKinematic) : useGravity(useGravity), startVelocity(velocity), isKinematic(isKinematic) 
    {
        currentVelocity = startVelocity;
    };

    ~PhysicsComponent() override;

    void Reset() override;
}; 