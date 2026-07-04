#include "PlayerAnimationSystem.h"

#include "Managers/Managers.h"

void SwapAnimation(AnimationComponent& anim, uint16_t animID)
{
    if (anim.currentAnimation == animID) return;

    anim.currentAnimation = animID;
    anim.currentFrame = 0;
    anim.currentTime = 0;
    anim.direction = 1;
}

void PlayerAnimationSystem::Update(World &world)
{
    uint32_t requiredBits = PhysicsBit | AnimationBit;

    for (uint8_t e = 0; e < MAX_ENTITIES; e++)
    {
        if ((world.entities[e].mask & requiredBits) != requiredBits) continue;

        auto& physics = world.physics[e];
        auto& anim = world.animations[e];
        auto& set = world.animSets[e];

        if (physics.currentVelocity.Magnitude() > 0) SwapAnimation(anim, set.states[AnimState::Walk]);
        else SwapAnimation(anim, set.states[AnimState::Idle]);
    }
}