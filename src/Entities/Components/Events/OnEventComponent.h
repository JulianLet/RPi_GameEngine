#pragma once
#include "Systems/Events/Event.h"

#include <unordered_map>
#include <functional>

struct OnEventComponent
{
    std::unordered_map<EventType, std::function<void(const Event&)>> events;
};