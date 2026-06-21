#pragma once

#include "Managers/Event/EventListener.h"

struct World;
class Game;

class EventComponentSystem : public EventListener
{
    World* world;

    public:
    EventComponentSystem(World& world, Game* game);
    ~EventComponentSystem() = default;

    void HandleEvent(const Event& event);
};