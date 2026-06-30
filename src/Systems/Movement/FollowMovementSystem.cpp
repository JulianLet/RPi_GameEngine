#include "FollowMovementSystem.h"

#include "Managers/Game/World.h"

void FollowMovementSystem::Update(World& world, float deltaTime)
{
    for (uint8_t e = 0; e < MAX_ENTITIES; e++)
    {
        if (!world.entities[e].isAlive) continue;
        uint32_t requiredMask = TransformBit | MovementBit | FollowTargetBit;

        if ((world.entities[e].mask & requiredMask) != requiredMask) continue;

        auto& transform = world.transforms[e];
        auto& movement = world.movements[e];
        auto& follow = world.followTargets[e];

        Vector2 diff = follow.target->currentPosition - transform.currentPosition;

        float lerpFactor = movement.speed * deltaTime;

        // Clamp lerpFactor to [0,1] to avoid overshooting
        if (lerpFactor > 1.0f) lerpFactor = 1.0f;

        transform.currentPosition += diff * lerpFactor;
    }
}
