#pragma once
#include "Entities/Component.h"
#include "Systems/Events/Event.h"

#include <unordered_map>
#include <functional>

struct OnEventComponent : public Component
{
    std::unordered_map<EventType, std::function<void(const Event&)>> events;

    OnEventComponent() = default;
    ~OnEventComponent() override;

    void Reset() override;
};