#include "PhysicsSystem.h"


#include "Entities/Entity.h"
#include "Entities/Components/Core/TransformComponent.h"
#include "Entities/Components/Core/MovementComponent.h"

#include "Entities/Components/Physics/PhysicsComponent.h"
#include "Entities/Components/Physics/PhysicsMaterialComponent.h"
#include "Entities/Components/Physics/ColliderComponent.h"

#include <unordered_set>
#include <cmath>

inline int sign(float v) { return (v > 0.f) - (v < 0.f); }

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
            physics->currentVelocity.y += 300 * deltaTime; //add gravity acceleration
        }

        //move entity
        transform->currentPosition.x += physics->currentVelocity.x * deltaTime; //velocity shoudld have speed in
        transform->currentPosition.y += physics->currentVelocity.y * deltaTime;
    }
}

void PhysicsSystem::ResolveCollisions(const std::vector<std::unique_ptr<Entity>> &entities, float deltaTime)
{
    std::unordered_set<Entity*> alreadyChecked;

    for (auto& entity : entities)
    {
        alreadyChecked.insert(entity.get()); //add to checked list

        ColliderComponent* myCollider = entity->GetComponent<ColliderComponent>();
        PhysicsComponent* myPhysics = entity->GetComponent<PhysicsComponent>();

        if (!myCollider || !myPhysics) continue;


        for (auto& other : myCollider->currentCollisions)
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
                CollisionKinematicStatic(entity.get(), other.first, other.second.timeOfCollision);
            }
            else if (myType == PhysicsType::STATIC && otherType == PhysicsType::KINEMATIC)
            {
                CollisionKinematicStatic(other.first, entity.get(), other.second.timeOfCollision);
            }

            //both dynamic
            else if (myType == PhysicsType::DYNAMIC && otherType == PhysicsType::DYNAMIC)
            {
                CollisionDynamicDynamic(entity.get(), other.first, other.second.timeOfCollision, other.second.collisionNormal, deltaTime);
            }

            //one dynamic
            else if (myType == PhysicsType::DYNAMIC && otherType != PhysicsType::DYNAMIC)
            {
                CollisionOneDynamic(entity.get(), other.first, other.second.timeOfCollision, other.second.collisionNormal, deltaTime);
            }
            else if (otherType == PhysicsType::DYNAMIC && myType != PhysicsType::DYNAMIC)
            {
                CollisionOneDynamic(other.first, entity.get(), other.second.timeOfCollision, other.second.collisionNormal, deltaTime);
            }
        }
    }
}


void PhysicsSystem::CollisionOneDynamic(Entity* dynamic, Entity* other, float& timeOfCollision, Vector2& normal, float deltaTime)
{
    auto dynamicTransform = dynamic->GetComponent<TransformComponent>();
    auto dynamicPhysics = dynamic->GetComponent<PhysicsComponent>();
    auto dynamicPM = dynamic->GetComponent<PhysicsMaterialComponent>();

    auto otherPM = other->GetComponent<PhysicsMaterialComponent>();

    float bounciness = 0.f;
    if (dynamicPM) bounciness += dynamicPM->bounciness;
    if (otherPM)   bounciness += otherPM->bounciness;
    bounciness /= 2.f;

    // Move to exact collision point
    dynamicTransform->currentPosition = dynamicTransform->lastPosition + dynamicPhysics->currentVelocity * timeOfCollision * deltaTime;

    // Reflect along normal with bounciness
    Vector2 v = dynamicPhysics->currentVelocity;
    dynamicPhysics->currentVelocity = v - 2 * Dot(v, normal) * normal * bounciness;

    // Small nudge to prevent sticking
    dynamicTransform->currentPosition += normal * 0.1f;
}


void PhysicsSystem::CollisionKinematicStatic(Entity* kinematic, Entity* other, float& timeOfCollision)
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


void PhysicsSystem::CollisionDynamicDynamic(Entity* a, Entity* b, float& timeOfCollision, Vector2& normal, float deltaTime)
{
    auto aTransform = a->GetComponent<TransformComponent>();
    auto bTransform = b->GetComponent<TransformComponent>();
    auto aPhysics = a->GetComponent<PhysicsComponent>();
    auto bPhysics = b->GetComponent<PhysicsComponent>();
    auto aPM = a->GetComponent<PhysicsMaterialComponent>();
    auto bPM = b->GetComponent<PhysicsMaterialComponent>();

    float bounciness = 0.f;
    if (aPM) bounciness += aPM->bounciness;
    if (bPM) bounciness += bPM->bounciness;
    bounciness /= 2.f;

    float mA = 1; // mass for future use
    float mB = 1;

    // Move both to collision point
    aTransform->currentPosition = aTransform->lastPosition + aPhysics->currentVelocity * timeOfCollision * deltaTime;
    bTransform->currentPosition = bTransform->lastPosition + bPhysics->currentVelocity * timeOfCollision * deltaTime;

    // Elastic collision formulas along normal
    if (normal.x != 0)
    {
        float vA = aPhysics->currentVelocity.x;
        float vB = bPhysics->currentVelocity.x;

        aPhysics->currentVelocity.x = ((mA - bounciness * mB) * vA + (1 + bounciness) * mB * vB) / (mA + mB);
        bPhysics->currentVelocity.x = ((mB - bounciness * mA) * vB + (1 + bounciness) * mA * vA) / (mA + mB);
    }

    if (normal.y != 0)
    {
        float vA = aPhysics->currentVelocity.y;
        float vB = bPhysics->currentVelocity.y;

        aPhysics->currentVelocity.y = ((mA - bounciness * mB) * vA + (1 + bounciness) * mB * vB) / (mA + mB);
        bPhysics->currentVelocity.y = ((mB - bounciness * mA) * vB + (1 + bounciness) * mA * vA) / (mA + mB);
    }

    // Nudge to prevent sticking
    aTransform->currentPosition += normal * 0.05f;
    bTransform->currentPosition -= normal * 0.05f;
}


Vector2 PhysicsSystem::GetOverlaps(TransformComponent* self, TransformComponent* other)
{
    float selfLeft   = self->currentPosition.x;
    float selfRight  = self->currentPosition.x + self->currentSize.x;
    float selfTop    = self->currentPosition.y;
    float selfBottom = self->currentPosition.y + self->currentSize.y;

    float otherLeft   = other->currentPosition.x;
    float otherRight  = other->currentPosition.x + other->currentSize.x;
    float otherTop    = other->currentPosition.y;
    float otherBottom = other->currentPosition.y + other->currentSize.y;

    // Compute overlap
    float overlapX = std::min(selfRight, otherRight) - std::max(selfLeft, otherLeft);
    float overlapY = std::min(selfBottom, otherBottom) - std::max(selfTop, otherTop);

    // Ensure non-negative overlap
    if (overlapX < 0.f) overlapX = 0.f;
    if (overlapY < 0.f) overlapY = 0.f;

    return Vector2{overlapX, overlapY};
}
