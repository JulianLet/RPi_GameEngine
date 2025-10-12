#pragma once

#include "Events/EventListener.h"

#include <vector>
#include <memory>

class Entity;
class Game;

class EventComponentSystem : public EventListener
{
    std::vector<std::unique_ptr<Entity>>* entities;

    public:
    EventComponentSystem(std::vector<std::unique_ptr<Entity>>& entities, Game* game);
    ~EventComponentSystem() = default;

    void HandleEvent(const Event& event);
};