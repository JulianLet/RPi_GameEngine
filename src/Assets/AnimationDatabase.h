#pragma once

#include "Globals.h"
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


struct AnimationDatabase
{
    AnimationClip clips[MAX_ANIMATIONS];
    bool filled[MAX_ANIMATIONS] = {false};
};