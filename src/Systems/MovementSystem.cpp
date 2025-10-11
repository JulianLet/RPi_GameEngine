#include "Systems/MovementSystem.h"
#include "Entities/Entity.h"
#include "Entities/Components/Transform.h"
#include "Entities/Components/Physics.h"
#include "Entities/Components/Movement.h"
#include "Entities/Components/InputIntend.h"
#include "Entities/Components/FollowTarget.h"

void MovementSystem::Update(const std::vector<std::unique_ptr<Entity>> &entities, float deltaTime)
{
    for (auto& entity : entities)
    {
        TransformComponent* transform = entity->GetComponent<TransformComponent>();
        MovementComponent* movement = entity->GetComponent<MovementComponent>();
        PhysicsComponent* physics = entity->GetComponent<PhysicsComponent>();
        InputIntendComponent* intend = entity->GetComponent<InputIntendComponent>();

        if (!movement) continue;

        if (transform)
        {
            transform->lastPosition = transform->currentPosition;
        }

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

        FollowTargetComponent* follow = entity->GetComponent<FollowTargetComponent>();

        if (transform && follow)
        {
            Vector2 diff = follow->target->currentPosition - transform->currentPosition;

            // Lerp factor controls how quickly we approach the target.
            float lerpFactor = movement->currentSpeed * deltaTime;

            // Clamp lerpFactor to [0,1] to avoid overshooting
            if (lerpFactor > 1.0f) lerpFactor = 1.0f;

            // Lerp between current position and target
            transform->currentPosition += diff * lerpFactor;
        }

    }
}
