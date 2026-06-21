#include "InputMovementSystem.h"

#include "Managers/Game/World.h"

void InputMovementSystem::Update(World& world, float deltaTime)
{
    for (uint8_t e = 0; e < MAX_ENTITIES; e++)
    {
        uint32_t requiredMask = PhysicsBit | InputIntentBit;

        if ((world.entities[e].mask & requiredMask) != requiredMask) continue;

        auto& physics = world.physics[e];
        auto& intend = world.inputIntends[e];

        if (intend.hasX)
        {
            physics.currentVelocity.x = intend.x;
        }

        if (intend.hasY)
        {
            physics.currentVelocity.y = intend.y;
        }
    }
}
