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
    if (relativeVel.x == 0 && relativeVel.y == 0)
    {
        // simple static AABB overlap test
        bool overlapX = std::abs(aT->currentPosition.x - bT->currentPosition.x) <= (aC->size.x + bC->size.x);
        bool overlapY = std::abs(aT->currentPosition.y - bT->currentPosition.y) <= (aC->size.y + bC->size.y);
        if (overlapX && overlapY)
        {
            intersection = aT->currentPosition; // or midpoint
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


    if (relativeVel.x > EPS) //moving right
    {
        entryX = (leftSide - aT->lastPosition.x) / relativeVel.x;
        exitX = (rightSide - aT->lastPosition.x) / relativeVel.x;
    }
    else if (relativeVel.x < -EPS) //moving left
    {
        entryX = (rightSide - aT->lastPosition.x) / relativeVel.x;
        exitX = (leftSide - aT->lastPosition.x) / relativeVel.x;
    }
    else
    {
        entryX = -FLT_MAX;
        exitX = FLT_MAX;
    }

    //projection on y
    float entryY = 0;
    float exitY = 0;

    if (relativeVel.y > EPS) //moving down
    {
        entryY = (topSide - aT->lastPosition.y) / relativeVel.y;
        exitY = (bottomSide - aT->lastPosition.y) / relativeVel.y;
    }
    else if (relativeVel.y < -EPS) //moving up
    {
        entryY = (bottomSide - aT->lastPosition.y) / relativeVel.y;
        exitY = (topSide - aT->lastPosition.y) / relativeVel.y;
    }
    else
    {
        entryY = -FLT_MAX;
        exitY = FLT_MAX;
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
