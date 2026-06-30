#include "EventComponentSystem.h"

#include "Globals.h"
#include "Managers/Event/EventManager.h"

#include "Games/Game.h"

#include "Managers/Event/Event.h"
#include "Managers/Game/World.h"

EventComponentSystem::EventComponentSystem(World& world, Game* game)
    : world(&world)
{
    EventManager::GetInstance().AddListener(this, game);
}

void EventComponentSystem::HandleEvent(const Event &event)
{
    for (uint8_t e = 0; e < MAX_ENTITIES; e++)
    {
        if (!world->entities[e].isAlive) continue;
        if (!world->entities[e].mask & OnEventBit) continue;

        auto& onEvent = world->events[e];

        auto it = onEvent.events.find(event.GetEventType());

        if (it != onEvent.events.end())
            it->second(event);
    }
}
