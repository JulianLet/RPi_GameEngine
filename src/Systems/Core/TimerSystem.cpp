#include "TimerSystem.h"

#include "Entities/Entity.h"
#include "Entities/Components/Core/TimerComponent.h"

void TimerSystem::Update(const std::vector<std::unique_ptr<Entity>> &entities, float deltaTime)
{
    for (auto& entity : entities)
    {
        auto* timer = entity->GetComponent<TimerComponent>();

        if (timer)
        {
            timer->currentTime += deltaTime;
        }

        //timed events
        //currenttime-= deltaTime
        //if currentime < 0
        //Eventsystem call event on countown
    }
}