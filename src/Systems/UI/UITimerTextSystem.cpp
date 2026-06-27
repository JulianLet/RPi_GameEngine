#include "UITimerTextSystem.h"

#include "Managers/Managers.h"

#include "Globals.h"
#include "Common.hpp"

#include <string>

void UITimerTextSystem::Update(World &world)
{
    uint32_t requiredMask = TimerBit | UITextBit;

    for (uint8_t e = 0; e < MAX_ENTITIES; e++)
    {
        if ((world.entities[e].mask & requiredMask) != requiredMask) continue;

        auto& timer = world.timers[e];
        auto& text = world.uiTexts[e];

        text.text = std::to_string(timer.currentTime);
    }
}