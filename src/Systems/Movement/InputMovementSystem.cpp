#include "InputMovementSystem.h"

#include <string>

#include "Managers/Game/World.h"
#include "Managers/Managers.h"

void InputMovementSystem::Update(World& world, float deltaTime)
{
    uint32_t requiredMask = PhysicsBit | MovementBit | InputIntentBit;

    for (uint8_t e = 0; e < MAX_ENTITIES; e++)
    {
        if (!world.entities[e].isAlive) continue;
        if ((world.entities[e].mask & requiredMask) != requiredMask) continue;

        auto& movement = world.movements[e];
        auto& physics = world.physics[e];
        auto& intend = world.inputIntends[e];

        if (intend.hasX)
        {
            physics.currentVelocity.x = intend.x * movement.speed;
        }

        if (intend.hasY)
        {
            physics.currentVelocity.y = intend.y * movement.speed;
        }
    }
}
