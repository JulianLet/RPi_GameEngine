#pragma once
#include "Managers/Event/Event.h"

#include <unordered_map>
#include <functional>

struct OnEventComponent
{
    std::unordered_map<EventType, std::function<void(const Event&)>> events;
};