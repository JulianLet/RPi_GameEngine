#include "CollisionSystem.h"

#include "Globals.h"
#include "Entities/Entity.h"
#include "Entities/Components/Core/TransformComponent.h"
#include "Entities/Components/Physics/ColliderComponent.h"
#include "Entities/Components/Physics/CollisionResponseComponent.h"

#include <cfloat>
#include <algorithm>

void CollisionSystem::Update(const std::vector<std::unique_ptr<Entity>>& entities)
{
    // Cleanup invalid collisions
    for (auto& entity : entities)
    {
        auto collider = entity->GetComponent<ColliderComponent>();
        if (!collider) continue;

        for (auto it = collider->currentCollisionsPos.begin(); it != collider->currentCollisionsPos.end(); )
        {
            if (it->first == nullptr)
                it = collider->currentCollisionsPos.erase(it);
            else
                ++it;
        }
    }

    for (int i = 0; i < entities.size(); i++)
    {
        auto transformA = entities[i]->GetComponent<TransformComponent>();
        auto colliderA = entities[i]->GetComponent<ColliderComponent>();

        if (!transformA || !colliderA) continue;

        for (int j = i + 1; j < entities.size(); j++)
        {
            auto transformB = entities[j]->GetComponent<TransformComponent>();
            auto colliderB = entities[j]->GetComponent<ColliderComponent>();

            if (!transformB || !colliderB) continue;
            // if (colliderA->isStatic && colliderB->isStatic) continue;

            Vector2 collisionPos = {0, 0};
            bool overlap = SweptAABB(transformA, colliderA, transformB, colliderB, collisionPos);
            
            if (!overlap && CheckOverlap(transformA, colliderA, transformB, colliderB)) 
            {
                collisionPos = transformA->currentPosition; // or midpoint of overlap
                overlap = true;
            }


            if (overlap)
            {
                HandleEnterStayPos(entities[i].get(), colliderA, entities[j].get(), collisionPos);
                HandleEnterStayPos(entities[j].get(), colliderB, entities[i].get(), collisionPos);
            }
            else
            {
                HandleExitPos(entities[i].get(), colliderA, entities[j].get());
                HandleExitPos(entities[j].get(), colliderB, entities[i].get());
            }
        }
    }
}

#include "CollisionSystem.h"

bool CollisionSystem::CheckOverlap(TransformComponent* aT, ColliderComponent* aC, TransformComponent* bT, ColliderComponent* bC)
{
    // Axis-Aligned Bounding Box overlap test
    return (aT->currentPosition.x < bT->currentPosition.x + bC->size.x &&
            aT->currentPosition.x + aC->size.x > bT->currentPosition.x &&
            aT->currentPosition.y < bT->currentPosition.y + bC->size.y &&
            aT->currentPosition.y + aC->size.y > bT->currentPosition.y);
}

bool CollisionSystem::SweptAABB(TransformComponent* aT, ColliderComponent* aC, TransformComponent* bT, ColliderComponent* bC, Vector2& intersection)
{
    // Calculate relative motion
    Vector2 motion = (aT->currentPosition - aT->lastPosition) - (bT->currentPosition - bT->lastPosition);

    // Expanded target rectangle
    float expandedLeft   = bT->currentPosition.x - aC->size.x;
    float expandedTop    = bT->currentPosition.y - aC->size.y;
    float expandedRight  = bT->currentPosition.x + bC->size.x;
    float expandedBottom = bT->currentPosition.y + bC->size.y;

    // Avoid divide by zero
    Vector2 invEntry, invExit;
    if (motion.x > 0.0f) {
        invEntry.x = expandedLeft - aT->lastPosition.x;
        invExit.x  = expandedRight - aT->lastPosition.x;
    } else {
        invEntry.x = expandedRight - aT->lastPosition.x;
        invExit.x  = expandedLeft - aT->lastPosition.x;
    }

    if (motion.y > 0.0f) {
        invEntry.y = expandedTop - aT->lastPosition.y;
        invExit.y  = expandedBottom - aT->lastPosition.y;
    } else {
        invEntry.y = expandedBottom - aT->lastPosition.y;
        invExit.y  = expandedTop - aT->lastPosition.y;
    }

    Vector2 entry, exit;
    entry.x = (motion.x == 0.0f) ? -FLT_MAX : invEntry.x / motion.x;
    exit.x  = (motion.x == 0.0f) ?  FLT_MAX : invExit.x  / motion.x;
    entry.y = (motion.y == 0.0f) ? -FLT_MAX : invEntry.y / motion.y;
    exit.y  = (motion.y == 0.0f) ?  FLT_MAX : invExit.y  / motion.y;

    float entryTime = std::max(entry.x, entry.y);
    float exitTime  = std::min(exit.x, exit.y);

    if (entryTime > exitTime || entryTime < 0.0f || entryTime > 1.0f)
        return false; // no collision

    // Collision occurs
    intersection = aT->lastPosition + motion * entryTime;
    return true;
}


void CollisionSystem::HandleEnterStayPos(Entity *self, ColliderComponent *collider, Entity *other, Vector2& intersection)
{
    auto response = self->GetComponent<CollisionResponseComponent>();

    auto [it, inserted] = collider->currentCollisionsPos.insert({other, intersection});

    if (!inserted)
    {
        it->second = intersection;
        if (response && response->OnStay)
            response->OnStay(self, other);
    }
    else
    {
        if (response && response->OnEnter)
            response->OnEnter(self, other);
    }
}

void CollisionSystem::HandleExitPos(Entity *self, ColliderComponent *collider, Entity *other)
{
    auto response = self->GetComponent<CollisionResponseComponent>();

    if (collider->currentCollisionsPos.erase(other))
    {
        if (response && response->OnExit)
            response->OnExit(self, other);
    }
}
