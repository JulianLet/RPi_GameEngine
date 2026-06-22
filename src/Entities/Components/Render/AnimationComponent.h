#pragma once
#include <cstdint>

//Requires Sprite Component
struct AnimationComponent
{
    uint16_t currentAnimation = 0;
    uint16_t currentFrame = 0;

    float currentTime = 0.0f;
    int direction = 1;

    bool active = true;
};