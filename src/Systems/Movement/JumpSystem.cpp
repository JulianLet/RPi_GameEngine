#include "JumpSystem.h"

#include "Managers/Game/World.h"
#include "Common.hpp"

void JumpSystem::Update(World &world, float deltaTime)
{
    uint32_t requiredMask = InputIntentBit | JumpBit | PhysicsBit;

    for (uint8_t e = 0; e < MAX_ENTITIES; e++)
    {
        if ((world.entities[e].mask & requiredMask) != requiredMask) continue;

        auto& intend = world.inputIntends[e];
        auto& jump = world.jumps[e];
        auto& physics = world.physics[e];
        
        if (intend.actions[InputAction::JUMP] && jump.isGrounded)
        {
            physics.currentVelocity.y = -jump.jumpPower;
        }
    }
}