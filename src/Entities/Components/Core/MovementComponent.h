#pragma once
#include "Entities/Component.h"
#include "Common.hpp"

struct MovementComponent : public Component
{
    float startSpeed = 0;
    float currentSpeed = 0;

    MovementComponent(float speed) : startSpeed(speed)
    {
        currentSpeed = startSpeed;
    };

    ~MovementComponent() override;

    void Reset() override;
};