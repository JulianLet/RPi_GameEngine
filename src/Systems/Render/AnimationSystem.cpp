#include "AnimationSystem.h"

#include "Hardware/Renderer.h"

#include "Entities/Entity.h"
#include "Entities/Components/Render/AnimationComponent.h"

#include "Systems/Debug/DebugManager.h"

void AnimationSystem::Update(const std::vector<std::unique_ptr<Entity>> &entities, float deltaTime)
{
    for (auto& entity : entities)
    {
        auto* animation = entity->GetComponent<AnimationComponent>();

        if (!animation) continue;
        if (animation->direction == 0) continue;
        
        animation->currentTime += deltaTime;

        if (animation->currentTime >= (1.0f / animation->FPS))
        {
            auto it = animation->animationList.find(animation->currentAnimation);
            if (it == animation->animationList.end()) continue; // not found
            
            Animation& currentAnimation = it->second;
            
            currentAnimation.currentFrame += animation->direction;
            animation->currentTime = 0;
            
            printf("increased Frame: %d of %d\n", currentAnimation.currentFrame, currentAnimation.frameAmount);

            if (currentAnimation.currentFrame < 0)
            {
                currentAnimation.currentFrame = 1;
                animation->direction = 1;
            }
            else if (currentAnimation.currentFrame >= currentAnimation.frameAmount)
            {
                switch (currentAnimation.mode)
                {
                case AnimationMode::LOOP:
                    currentAnimation.currentFrame = 0;
                    break;

                case AnimationMode::BOUNCE:
                    currentAnimation.currentFrame = currentAnimation.frameAmount - 2;
                    animation->direction = -1;
                    break;

                case AnimationMode::SINGLE:
                    currentAnimation.currentFrame = currentAnimation.frameAmount - 1;
                    animation->direction = 0; //stop
                    break;
                }
            }
        }
    }
}