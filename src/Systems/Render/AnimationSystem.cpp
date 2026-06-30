#include "AnimationSystem.h"
#include "Managers/Game/World.h"

#include <cstdint>

void AnimationSystem::Update(World& world, float dt)
{
    uint32_t requiredMask = AnimationBit | SpriteBit;

    for (uint8_t e = 0; e < MAX_ENTITIES; e++)
    {
        if (!world.entities[e].isAlive) continue;
        if ((world.entities[e].mask & requiredMask) != requiredMask) continue;

        auto& anim = world.animations[e];
        auto& sprite = world.sprites[e];

        if (!anim.active) continue;

        AnimationClip& clip = world.assets.myAnimations.clips[anim.currentAnimation];

        // advance time
        anim.currentTime += dt;

        float frameTime = 1.0f / clip.fps;

        if (anim.currentTime < frameTime) continue;

        anim.currentTime = 0.0f;

        // advance frame
        anim.direction; // kept for bounce logic

        uint16_t& frameIndex = anim.currentFrame;

        frameIndex += anim.direction;

        // boundary handling
        if (frameIndex < 0)
        {
            frameIndex = 1;
            anim.direction = 1;
        }
        else if (frameIndex >= clip.frameCount)
        {
            switch (clip.mode)
            {
                case LOOP:
                    frameIndex = 0;
                    break;

                case BOUNCE:
                    frameIndex = clip.frameCount - 2;
                    anim.direction = -1;
                    break;

                case SINGLE:
                    frameIndex = clip.frameCount - 1;
                    anim.active = false;
                    break;
            }
        }

        // compute final sprite frame index
        anim.currentFrame = frameIndex;
        sprite.spriteID = clip.frameStart + anim.currentFrame;
    }
}