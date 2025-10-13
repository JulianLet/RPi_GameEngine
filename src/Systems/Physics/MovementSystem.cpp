#include "MovementSystem.h"

#include "Entities/Entity.h"
#include "Entities/Components/Core/TransformComponent.h"
#include "Entities/Components/Core/MovementComponent.h"

#include "Entities/Components/Physics/PhysicsComponent.h"
#include "Entities/Components/Input/InputIntendComponent.h"
#include "Entities/Components/Render/FollowTargetComponent.h"

void MovementSystem::Update(const std::vector<std::unique_ptr<Entity>> &entities, float deltaTime)
{
    for (auto& entity : entities)
    {
        TransformComponent* transform = entity->GetComponent<TransformComponent>();
        MovementComponent* movement = entity->GetComponent<MovementComponent>();
        PhysicsComponent* physics = entity->GetComponent<PhysicsComponent>();
        InputIntendComponent* intend = entity->GetComponent<InputIntendComponent>();

        if (!movement) continue;

        if (transform) //update old position before any movement
        {
            transform->lastPosition = transform->currentPosition;
        }

        //update velocity based on input
        if (physics && intend)
        {   
            if (intend->hasX)
            {
                physics->currentVelocity.x = intend->x;
            }

            if (intend->hasY)
            {
                physics->currentVelocity.y = intend->y;
            }
        }

        //lerp towards position based on to follow entity
        FollowTargetComponent* follow = entity->GetComponent<FollowTargetComponent>();

        if (transform && follow)
        {
            Vector2 diff = follow->target->currentPosition - transform->currentPosition;

            float lerpFactor = movement->currentSpeed * deltaTime;

            // Clamp lerpFactor to [0,1] to avoid overshooting
            if (lerpFactor > 1.0f) lerpFactor = 1.0f;

            transform->currentPosition += diff * lerpFactor;
        }

    }
}
