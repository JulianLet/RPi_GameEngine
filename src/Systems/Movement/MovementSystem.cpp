#include "MovementSystem.h"

#include "Managers/Game/World.h"
#include "Common.hpp"

void MovementSystem::Update(World& world, float deltaTime)
{
    uint32_t requiredMask = TransformBit | MovementBit;

    for (uint8_t e = 0; e < MAX_ENTITIES; e++)
    {
        if (!world.entities[e].isAlive) continue;
        if ((world.entities[e].mask & requiredMask) != requiredMask) continue;

        auto& transform = world.transforms[e];
        auto& movement = world.movements[e];
        
        transform.lastPosition = transform.currentPosition;
    }
}
