#pragma once
#include <cstdint>

struct AnimationComponent
{
    uint16_t currentAnimation = 0;

    float currentTime = 0.0f;
    int direction = 1;

    bool active = true;
};