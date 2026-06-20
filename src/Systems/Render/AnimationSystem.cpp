#include "AnimationSystem.h"
#include "Systems/Core/World.h"

void AnimationSystem::Update(World& world, float dt)
{
    for (uint8_t i = 0; i < MAX_ENTITIES; i++)
    {
        if (!world.entities[i].isAlive) continue;
        if (!(world.entities[i].mask & (1 << 5))) continue; // AnimationBit (adjust index)

        auto& anim = world.animations[i];

        if (!anim.active) continue;

        AnimationClip& clip =
            world.animationDB.clips[anim.currentAnimation];

        // advance time
        anim.currentTime += dt;

        float frameTime = 1.0f / clip.fps;

        if (anim.currentTime < frameTime)
            continue;

        anim.currentTime = 0.0f;

        // advance frame
        anim.direction; // kept for bounce logic

        u_int16_t& frameIndex = world.sprites[i].frameIndex;

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
        world.sprites[i].frame =
            clip.frameStart + frameIndex;
    }
}