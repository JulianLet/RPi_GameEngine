#include "EventComponentSystem.h"

#include "Globals.h"
#include "EventSystem.h"

#include "Games/Game.h"

#include "Entities/Entity.h"
#include "Entities/Components/Events/OnEventComponent.h"

EventComponentSystem::EventComponentSystem(std::vector<std::unique_ptr<Entity>>& entities, Game* game)
    : entities(&entities)
{
    EventSystem::GetInstance().AddListener(this, game);
}

void EventComponentSystem::HandleEvent(const Event &event)
{
    for (auto& entity : *entities)
    {
        auto* onEvent = entity->GetComponent<OnEventComponent>();

        if (!onEvent) continue;

        auto it = onEvent->events.find(event.GetEventType());

        if (it != onEvent->events.end())
            it->second(event);
    }
}
