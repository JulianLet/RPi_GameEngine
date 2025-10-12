#include "EventComponentSystem.h"
#include "EventSystem.h"

#include "Games/Game.h"
#include "Entities/Entity.h"

#include "Globals.h"
#include "Entities/Components/Rectangle.h"
#include "Entities/Components/OnEventComponent.h"

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
        auto* rectangle = entity->GetComponent<RectangleComponent>();

        // if (rectangle) rectangle->currentColor = Color::CYAN;
        if (!onEvent) continue;
        // if (rectangle) rectangle->currentColor = Color::RED;

        auto it = onEvent->events.find(event.GetEventType());

        if (it != onEvent->events.end())
            it->second(event);
    }
}
