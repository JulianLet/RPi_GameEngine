#pragma once
#include "Entities/Component.h"
#include "Common.h"

struct TimerComponent : public Component
{
    float currentTime = 0;

    TimerComponent() = default;

    ~TimerComponent() override;
    void Reset() override;
};