#include "PhysicsSystem.h"


#include "Entities/Entity.h"
#include "Entities/Components/Core/TransformComponent.h"
#include "Entities/Components/Core/MovementComponent.h"

#include "Entities/Components/Physics/PhysicsComponent.h"
#include "Entities/Components//Physics/PhysicsMaterialComponent.h"
#include "Entities/Components/Physics/ColliderComponent.h"

#include <unordered_set>
#include <cmath>


void PhysicsSystem::Update(const std::vector<std::unique_ptr<Entity>> &entities, float deltaTime)
{
    for (auto& entity : entities)
    {
        TransformComponent* transform = entity->GetComponent<TransformComponent>();
        PhysicsComponent* physics = entity->GetComponent<PhysicsComponent>();
        MovementComponent* movement = entity->GetComponent<MovementComponent>();

        if (!transform || !physics || !movement) continue;

        if (physics->useGravity)
        {
            physics->currentVelocity.y += 10 * deltaTime; //add gravity acceleration
        }

        //move entity
        transform->currentPosition.x += physics->currentVelocity.x * movement->currentSpeed * deltaTime;
        transform->currentPosition.y += physics->currentVelocity.y * movement->currentSpeed * deltaTime;
    }
}

void PhysicsSystem::ResolveCollisions(const std::vector<std::unique_ptr<Entity>> &entities)
{
    std::unordered_set<Entity*> alreadyChecked;

    for (auto& entity : entities)
    {
        alreadyChecked.insert(entity.get()); //add to checked list

        ColliderComponent* myCollider = entity->GetComponent<ColliderComponent>();
        PhysicsComponent* myPhysics = entity->GetComponent<PhysicsComponent>();

        if (!myCollider || !myPhysics) continue;


        for (auto& other : myCollider->currentCollisionsPos)
        {
            if (alreadyChecked.find(other.first) != alreadyChecked.end()) continue; //skip if already checked

            ColliderComponent* otherCollider = other.first->GetComponent<ColliderComponent>();
            PhysicsComponent* otherPhysics = other.first->GetComponent<PhysicsComponent>();

            if (!otherCollider || !otherPhysics) continue;
            if (myCollider->isTrigger || otherCollider->isTrigger) continue; //skip if only trigger collision

            const auto myType    = myPhysics->physicsType;
            const auto otherType = otherPhysics->physicsType;

            if (myType == PhysicsType::STATIC && otherType == PhysicsType::STATIC)
                continue;

            //one kin one static
            if (myType == PhysicsType::KINEMATIC && otherType == PhysicsType::STATIC)
            {
                CollisionKinematicStatic(entity.get(), other.first);
            }
            else if (myType == PhysicsType::STATIC && otherType == PhysicsType::KINEMATIC)
            {
                CollisionKinematicStatic(other.first, entity.get());
            }

            //both dynamic
            else if (myType == PhysicsType::DYNAMIC && otherType == PhysicsType::DYNAMIC)
            {
                CollisionDynamicDynamic(entity.get(), other.first);
            }

            //one dynamic
            else if (myType == PhysicsType::DYNAMIC && otherType != PhysicsType::DYNAMIC)
            {
                CollisionOneDynamic(entity.get(), other.first);
            }
            else if (otherType == PhysicsType::DYNAMIC && myType != PhysicsType::DYNAMIC)
            {
                CollisionOneDynamic(other.first, entity.get());
            }
        }
    }
}

void PhysicsSystem::CollisionOneDynamic(Entity* dynamic, Entity* other)
{
    TransformComponent* dynamicTransform = dynamic->GetComponent<TransformComponent>();
    PhysicsComponent* dynamicPhysics = dynamic->GetComponent<PhysicsComponent>();
    PhysicsMaterialComponent* dynamicPM = dynamic->GetComponent<PhysicsMaterialComponent>();
    
    TransformComponent* otherTransform = other->GetComponent<TransformComponent>();
    PhysicsMaterialComponent* otherPM = other->GetComponent<PhysicsMaterialComponent>();

    Vector2 overlaps = GetOverlaps(dynamicTransform, otherTransform);

    //if contact only apply friction
    if (overlaps.x <= 1.f && overlaps.y <= 1.f) 
    {
        float friction = dynamicPM ? dynamicPM->friction : 0;
        friction += otherPM ? otherPM->friction : 0;

        float frictionFactor = 1 - friction;
        dynamicPhysics->currentVelocity = dynamicPhysics->currentVelocity * frictionFactor;
        return;
    }

    //add bounces
    float sB = dynamicPM ? dynamicPM->bounciness : 0;
    float oB = otherPM ? otherPM->bounciness : 0;
    float aB = (sB + oB) / 2.f; //average bounciness

    if (overlaps.x < overlaps.y)
    {
        //direction agains the old
        int dir = dynamicPhysics->currentVelocity.x < 0 ? 1 : -1;
        dynamicTransform->currentPosition.x += dir * overlaps.x;
        dynamicPhysics->currentVelocity.x *= -aB;
    }
    else
    {
        //direction agains the old
        int dir = dynamicPhysics->currentVelocity.y < 0 ? 1 : -1;
        dynamicTransform->currentPosition.y += dir * overlaps.y;
        dynamicPhysics->currentVelocity.y *= -aB;
    }
}

void PhysicsSystem::CollisionKinematicStatic(Entity* kinematic, Entity* other)
{
    TransformComponent* kTransform = kinematic->GetComponent<TransformComponent>();
    TransformComponent* oTransform = other->GetComponent<TransformComponent>();

    Vector2 overlaps = GetOverlaps(kTransform, oTransform);

    if (overlaps.x <= 1.f && overlaps.y <= 1.f) return;

    // Move kinematic out of overlap along smallest axis
    if (overlaps.x < overlaps.y)
    {
        //direction agains the old
        int dir = kinematic->GetComponent<PhysicsComponent>()->currentVelocity.x < 0 ? 1 : -1;
        kTransform->currentPosition.x += dir * overlaps.x;
    }
    else
    {
        //direction agains the old
        int dir = kinematic->GetComponent<PhysicsComponent>()->currentVelocity.y < 0 ? 1 : -1;
        kTransform->currentPosition.y += dir * overlaps.y;
    }
}


void PhysicsSystem::CollisionDynamicDynamic(Entity* self, Entity* other)
{
    TransformComponent* selfTransform = self->GetComponent<TransformComponent>();
    PhysicsComponent* selfPhysics = self->GetComponent<PhysicsComponent>();
    PhysicsMaterialComponent* selfPM = self->GetComponent<PhysicsMaterialComponent>();

    TransformComponent* otherTransform = other->GetComponent<TransformComponent>();
    PhysicsComponent* otherPhysics = other->GetComponent<PhysicsComponent>();
    PhysicsMaterialComponent* otherPM = other->GetComponent<PhysicsMaterialComponent>();

    Vector2 overlaps = GetOverlaps(selfTransform, otherTransform);

    //resolve overlaps
    if (overlaps.x < overlaps.y)
    {
        //direction agains the old
        float dir = selfPhysics->currentVelocity.x < 0 ? 0.5f : -0.5f;
        selfTransform->currentPosition.x += dir * overlaps.x;
        otherTransform->currentPosition.x -= dir * overlaps.x;
    }
    else
    {
        //direction agains the old
        float dir = selfPhysics->currentVelocity.y < 0 ? 0.5f : -0.5f;
        selfTransform->currentPosition.y += dir * overlaps.y;
        otherTransform->currentPosition.y -= dir * overlaps.y;
    }

    //add bounces
    float sB = selfPM ? selfPM->bounciness : 0;
    float oB = otherPM ? otherPM->bounciness : 0;
    float aB = (sB + oB) / 2.f; //average bounciness

    //mass for now at 1
    float sM = 1.f;
    float oM = 1.f;
    
    Vector2 sV = selfPhysics->currentVelocity;
    Vector2 oV = otherPhysics->currentVelocity;

    //elastic collision formulas
    //vA' = ((mA - boun*mB) * vA + (1 + bounc) * mB * vB) / (mA + mB)
    //vB' = ((mB - bounc*mA) * vB + (1 + bounc) * mA * vA) / (mA + mB)

    if (overlaps.x < overlaps.y)
    {
        selfPhysics->currentVelocity.x = ((sM - aB * oM) * sV.x + (1 + aB) * oM * oV.x) / (sM + oM);
        otherPhysics->currentVelocity.x = ((oM - aB * sM) * oV.x + (1 + aB) * sM * sV.x) / (oM + sM);
    }
    else
    {
        selfPhysics->currentVelocity.y = ((sM - aB * oM) * sV.y + (1 + aB) * oM * oV.y) / (sM + oM);
        otherPhysics->currentVelocity.y = ((oM - aB * sM) * oV.y + (1 + aB) * sM * sV.y) / (oM + sM);
    }
}

Vector2 PhysicsSystem::GetOverlaps(TransformComponent *self, TransformComponent *other)
{
    float overlapX = (self->currentSize.x + other->currentSize.x) / 2 - std::abs(self->GetCenterPos().x - other->GetCenterPos().x);
    float overlapY = (self->currentSize.y + other->currentSize.y) / 2 - std::abs(self->GetCenterPos().y - other->GetCenterPos().y);

    return Vector2{overlapX, overlapY};
}