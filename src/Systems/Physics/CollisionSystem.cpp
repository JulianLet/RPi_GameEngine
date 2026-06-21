#include "CollisionSystem.h"

#include "Globals.h"
#include "Managers/Game/World.h"

#include <cfloat>
#include <algorithm>

void CollisionSystem::Update(World& world)
{
    // Cleanup invalid collisions
    for (auto& entity : entities)
    {
        auto collider = entity->GetComponent<ColliderComponent>();
        if (!collider) continue;

        for (auto it = collider->currentCollisions.begin(); it != collider->currentCollisions.end(); )
        {
            if (it->first == nullptr)
                it = collider->currentCollisions.erase(it);
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

            float timeOfCollision = 0;
            Vector2 normal = {0, 0};

            bool overlap = SweptAABB(transformA, colliderA, transformB, colliderB, timeOfCollision, normal);
            
            if (overlap)
            {
                HandleEnterStayPos(entities[i].get(), colliderA, entities[j].get(), timeOfCollision, normal);

                Vector2 normalForB = {-normal.x, -normal.y};
                HandleEnterStayPos(entities[j].get(), colliderB, entities[i].get(), timeOfCollision, normalForB);
            }
            else
            {
                HandleExitPos(entities[i].get(), colliderA, entities[j].get());
                HandleExitPos(entities[j].get(), colliderB, entities[i].get());
            }
        }
    }
}

bool CollisionSystem::SweptAABB(TransformComponent* aT, ColliderComponent* aC, TransformComponent* bT, ColliderComponent* bC, float& timeOfCollision, Vector2& normal)
{
    //little bit of tolerance
    const float EPS = 0.001;

    //reset variables
    timeOfCollision = 0;
    normal = {0,0};

    //calculate relative velocity
    Vector2 relativeVel = (aT->currentPosition - aT->lastPosition) - (bT->currentPosition - bT->lastPosition);

    //if there is no movement static collision check
    if (std::abs(relativeVel.x) < EPS && std::abs(relativeVel.y) < EPS)
    {
        // proper static AABB overlap check
        Vector2 overlaps = GetOverlaps(aT, bT);

        if (overlaps.x < EPS || overlaps.y < EPS) return false; // not enough penetration

        timeOfCollision = 0; //if no movement they were colliding at the beginning of the frame as well

        if (overlaps.x < overlaps.y) //smaller penetration
        {
            normal.x = (aT->currentPosition.x < bT->currentPosition.x) ? 1 : -1;
        }
        else
        {
            normal.y = (aT->currentPosition.y < bT->currentPosition.y) ? -1 : 1;
        }

        return true;
    }

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

    if (entryX > entryY) //hit is if the second axis enters
    {
        normal.x = (aT->GetCenterPos().x < bT->GetCenterPos().x) ? -1 : 1;
        // normal.x = (relativeVel.x < 0) ? 1 : -1;
    } 
    else
    {
        normal.y = (aT->GetCenterPos().y < bT->GetCenterPos().y) ? -1 : 1;
        // normal.y = (relativeVel.y < 0) ? 1 : -1;
    }

    if (entryTime < 0.f) entryTime = 0.f;
    timeOfCollision = entryTime;

    return true;
}

void CollisionSystem::HandleEnterStayPos(Entity *self, ColliderComponent *collider, Entity *other, float& timeOfCollision, Vector2& normal)
{
    auto response = self->GetComponent<CollisionResponseComponent>();
    // self->GetComponent<RectangleComponent>()->currentColor = Color::RED;

    auto [it, inserted] = collider->currentCollisions.insert({other, CollisionInfo{timeOfCollision, normal}});

    if (!inserted)
    {
        it->second = CollisionInfo{timeOfCollision, normal};
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
    
    if (collider->currentCollisions.erase(other))
    {
        // self->GetComponent<RectangleComponent>()->Reset();
        if (response && response->OnExit)
            response->OnExit(self, other);
    }
}

Vector2 CollisionSystem::GetOverlaps(TransformComponent* self, TransformComponent* other)
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
