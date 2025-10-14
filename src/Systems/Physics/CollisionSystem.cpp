#include "CollisionSystem.h"

#include "Globals.h"
#include "Entities/Entity.h"
#include "Entities/Components/Core/TransformComponent.h"
#include "Entities/Components/Render/RectangleComponent.h"
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

            Vector2 collisionPos = {0, 0};
            bool overlap = SweptAABB(transformA, colliderA, transformB, colliderB, collisionPos);
            
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

bool CollisionSystem::SweptAABB(TransformComponent* aT, ColliderComponent* aC, TransformComponent* bT, ColliderComponent* bC, Vector2& intersection)
{
    //little bit of tolerance
    const float EPS = 0.001;


    //calculate relative velocity
    Vector2 relativeVel = (aT->currentPosition - aT->lastPosition) - (bT->currentPosition - bT->lastPosition);

    //if there is no movement static collision check
    if (std::abs(relativeVel.x) < EPS && std::abs(relativeVel.y) < EPS)
    {
        // proper static AABB overlap check
        bool overlapX = (aT->currentPosition.x < bT->currentPosition.x + bC->size.x) &&
                        (aT->currentPosition.x + aC->size.x > bT->currentPosition.x);
        bool overlapY = (aT->currentPosition.y < bT->currentPosition.y + bC->size.y) &&
                        (aT->currentPosition.y + aC->size.y > bT->currentPosition.y);

        if (overlapX && overlapY)
        {
            intersection.x = (std::max(aT->currentPosition.x, bT->currentPosition.x) +
                            std::min(aT->currentPosition.x + aC->size.x, bT->currentPosition.x + bC->size.x)) / 2;
            intersection.y = (std::max(aT->currentPosition.y, bT->currentPosition.y) +
                            std::min(aT->currentPosition.y + aC->size.y, bT->currentPosition.y + bC->size.y)) / 2;
            return true;
        }
        return false;
    }


    //extend "static" rectangle. I choose b -> extended to top left
    float leftSide = bT->currentPosition.x - aC->size.x;
    float rightSide = bT->currentPosition.x + bC->size.x;
    float topSide = bT->currentPosition.y - aC->size.y;
    float bottomSide = bT->currentPosition.y + bC->size.y;

    //projection on x
    // ax + entryX * vx = leftSide -> entryX = (leftside - ax) / vx
    // ax + exitX * vx = rightSide -> exitX = (rightSide - ax) / vx

    float entryX = 0;
    float exitX = 0;

    // X-axis
    if (std::abs(relativeVel.x) < EPS)
    {
        // If no movement in X, check if overlapping; else no collision
        if (aT->lastPosition.x + aC->size.x <= bT->lastPosition.x ||
            aT->lastPosition.x >= bT->lastPosition.x + bC->size.x)
        {
            return false;
        }

        entryX = -FLT_MAX;
        exitX  = FLT_MAX;
    }
    else if (relativeVel.x > 0) // moving right
    {
        entryX = (bT->lastPosition.x - (aT->lastPosition.x + aC->size.x)) / relativeVel.x;
        exitX  = (bT->lastPosition.x + bC->size.x - aT->lastPosition.x) / relativeVel.x;
    }
    else // moving left
    {
        entryX = (bT->lastPosition.x + bC->size.x - aT->lastPosition.x) / relativeVel.x;
        exitX  = (bT->lastPosition.x - (aT->lastPosition.x + aC->size.x)) / relativeVel.x;
    }

    //projection on y
    float entryY = 0;
    float exitY = 0;

    // Y-axis
    if (std::abs(relativeVel.y) < EPS)
    {
        if (aT->lastPosition.y + aC->size.y <= bT->lastPosition.y ||
            aT->lastPosition.y >= bT->lastPosition.y + bC->size.y)
        {
            return false;
        }
        entryY = -FLT_MAX;
        exitY  = FLT_MAX;
    }
    else if (relativeVel.y > 0) // moving down
    {
        entryY = (bT->lastPosition.y - (aT->lastPosition.y + aC->size.y)) / relativeVel.y;
        exitY  = (bT->lastPosition.y + bC->size.y - aT->lastPosition.y) / relativeVel.y;
    }
    else // moving up
    {
        entryY = (bT->lastPosition.y + bC->size.y - aT->lastPosition.y) / relativeVel.y;
        exitY  = (bT->lastPosition.y - (aT->lastPosition.y + aC->size.y)) / relativeVel.y;
    }

    //get entry/ exit times
    float entryTime = std::max(entryX, entryY);
    float exitTime = std::min(exitX, exitY);

    //no collision if entry after 1, exit before 0 or exit before enter
    if (entryTime > 1.f || exitTime < 0 || entryTime > exitTime) return false;

    if (entryTime < 0.f) entryTime = 0.f;
    intersection = aT->lastPosition + entryTime * relativeVel;
    return true;
}

void CollisionSystem::HandleEnterStayPos(Entity *self, ColliderComponent *collider, Entity *other, Vector2& intersection)
{
    auto response = self->GetComponent<CollisionResponseComponent>();
    // self->GetComponent<RectangleComponent>()->currentColor = Color::RED;

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
        // self->GetComponent<RectangleComponent>()->Reset();
        if (response && response->OnExit)
            response->OnExit(self, other);
    }
}
