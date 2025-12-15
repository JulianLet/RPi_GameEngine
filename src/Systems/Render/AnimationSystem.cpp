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

        //check if should swap the animation
        bool swapped = false;
        if (auto it = animation->transitions.find(animation->currentAnimation); it != animation->transitions.end()) 
        {
            for (auto& transition : it->second) 
            {
                transition(swapped);

                if (swapped) break;
            }
        }

        if (swapped) continue;

        //check if it should progress the animation frames
        if (animation->direction == 0) continue;
        
        animation->currentTime += deltaTime;

        if (animation->currentTime >= (1.0f / animation->FPS))
        {
            auto it = animation->animationList.find(animation->currentAnimation);
            if (it == animation->animationList.end()) continue; // not found
            
            Animation* currentAnimation = it->second.get();
            
            currentAnimation->currentFrame += animation->direction;
            animation->currentTime = 0;
            
            if (currentAnimation->currentFrame < 0)
            {
                currentAnimation->currentFrame = 1;
                animation->direction = 1;
            }
            else if (currentAnimation->currentFrame >= currentAnimation->frameAmount)
            {
                switch (currentAnimation->mode)
                {
                case AnimationMode::LOOP:
                    currentAnimation->currentFrame = 0;
                    break;

                case AnimationMode::BOUNCE:
                    currentAnimation->currentFrame = currentAnimation->frameAmount - 2;
                    animation->direction = -1;
                    break;

                case AnimationMode::SINGLE:
                    currentAnimation->currentFrame = currentAnimation->frameAmount - 1;
                    animation->direction = 0; //stop
                    break;
                }
            }

            currentAnimation->UpdateFrame();
        }
    }
}