#include "TimerSystem.h"

#include "Managers/Game/World.h"
#include "Entities/Components/Core/TimerComponent.h"

void TimerSystem::Update(World& world, float deltaTime)
{
    for (uint8_t e = 0; e < MAX_ENTITIES; e++)
    {
        if (!world.entities[e].isAlive) continue;
        if (!world.entities[e].mask & TimerBit) continue;

        auto& timer = world.timers[e];
        
        timer.currentTime += deltaTime;

        //timed events
        //currenttime-= deltaTime
        //if currentime < 0
        //Eventsystem call event on countown
    }
}