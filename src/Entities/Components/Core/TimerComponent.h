#pragma once
#include "Entities/Component.h"
#include "Common.hpp"

struct TimerComponent : public Component
{
    float currentTime = 0;

    TimerComponent() = default;

    ~TimerComponent() override;
    void Reset() override;
};