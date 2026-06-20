#pragma once
#include <cstdint>

enum AnimationMode : uint8_t
{
    SINGLE = 0,
    BOUNCE,
    LOOP
};

struct AnimationClip
{
    uint16_t frameStart;
    uint16_t frameCount;
    uint8_t fps;
    AnimationMode mode;
};

constexpr int MAX_ANIMATIONS = 32;

struct AnimationDatabase
{
    AnimationClip clips[MAX_ANIMATIONS];
};

struct AnimationComponent
{
    uint16_t currentAnimation = 0;

    float currentTime = 0.0f;
    int direction = 1;

    bool active = true;
};