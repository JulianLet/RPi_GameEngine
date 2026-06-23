#include "CollisionSystem.h"

#include "Common.hpp"
#include "Globals.h"
#include "Managers/Game/World.h"

#include <cfloat>
#include <algorithm>

void CleanupCollisions(World& world)
{
    for (uint8_t i = 0; i < MAX_ENTITIES; i++)
    {
        if (!(world.entities[i].mask & ColliderBit)) continue;

        auto& col = world.colliders[i];

        for (int c = 0; c < MAX_COLLISIONS; c++)
        {
            if (col.currentCollisions[c].otherID == UINT8_MAX)
                continue;

            uint8_t other = col.currentCollisions[c].otherID;

            if (other >= MAX_ENTITIES || !(world.entities[other].mask & ColliderBit))
            {
                col.currentCollisions[c].otherID = UINT8_MAX;
            }
        }
    }
}

int FindCollisionSlot(ColliderComponent& col, uint8_t other)
{
    for (int i = 0; i < MAX_COLLISIONS; i++)
    {
        if (col.currentCollisions[i].otherID == other)
            return i;
    }
    return -1;
}

int AddCollision(ColliderComponent& col, uint8_t other, CollisionInfo info)
{
    for (int i = 0; i < MAX_COLLISIONS; i++)
    {
        if (col.currentCollisions[i].otherID == UINT8_MAX ||
            col.currentCollisions[i].otherID == 0)
        {
            col.currentCollisions[i] = { other, info };
            return i;
        }
    }
    return -1; // full
}

void RemoveCollision(ColliderComponent& col, uint8_t other)
{
    for (int i = 0; i < MAX_COLLISIONS; i++)
    {
        if (col.currentCollisions[i].otherID == other)
        {
            col.currentCollisions[i].otherID = UINT8_MAX;
            return;
        }
    }
}

Vector2 GetOverlaps(TransformComponent* self, TransformComponent* other)
{
    float selfLeft   = self->currentPosition.x;
    float selfRight  = self->currentPosition.x + self->currentSize.x;
    float selfTop    = self->currentPosition.y;
    float selfBottom = self->currentPosition.y + self->currentSize.y;

    float otherLeft   = other->currentPosition.x;
    float otherRight  = other->currentPosition.x + other->currentSize.x;
    float otherTop    = other->currentPosition.y;
    float otherBottom = other->currentPosition.y + other->currentSize.y;

    float overlapX = std::min(selfRight, otherRight) - std::max(selfLeft, otherLeft);
    float overlapY = std::min(selfBottom, otherBottom) - std::max(selfTop, otherTop);

    if (overlapX < 0.f) overlapX = 0.f;
    if (overlapY < 0.f) overlapY = 0.f;

    return Vector2{ overlapX, overlapY };
}

bool SweptAABB(
    TransformComponent* aT, ColliderComponent* aC,
    TransformComponent* bT, ColliderComponent* bC,
    float& timeOfCollision, Vector2& normal)
{
    const float EPS = 0.001f;

    timeOfCollision = 0;
    normal = {0,0};

    Vector2 relativeVel =
        (aT->currentPosition - aT->lastPosition) -
        (bT->currentPosition - bT->lastPosition);

    if (std::abs(relativeVel.x) < EPS && std::abs(relativeVel.y) < EPS)
    {
        Vector2 overlaps = GetOverlaps(aT, bT);

        if (overlaps.x < EPS || overlaps.y < EPS)
            return false;

        timeOfCollision = 0;

        if (overlaps.x < overlaps.y)
        {
            normal.x = (aT->currentPosition.x < bT->currentPosition.x) ? 1 : -1;
        }
        else
        {
            normal.y = (aT->currentPosition.y < bT->currentPosition.y) ? -1 : 1;
        }

        return true;
    }

    float entryX = 0;
    float exitX = 0;

    if (std::abs(relativeVel.x) < EPS)
    {
        if (aT->lastPosition.x + aC->size.x <= bT->lastPosition.x ||
            aT->lastPosition.x >= bT->lastPosition.x + bC->size.x)
        {
            return false;
        }

        entryX = -FLT_MAX;
        exitX  = FLT_MAX;
    }
    else if (relativeVel.x > 0)
    {
        entryX = (bT->lastPosition.x - (aT->lastPosition.x + aC->size.x)) / relativeVel.x;
        exitX  = (bT->lastPosition.x + bC->size.x - aT->lastPosition.x) / relativeVel.x;
    }
    else
    {
        entryX = (bT->lastPosition.x + bC->size.x - aT->lastPosition.x) / relativeVel.x;
        exitX  = (bT->lastPosition.x - (aT->lastPosition.x + aC->size.x)) / relativeVel.x;
    }

    float entryY = 0;
    float exitY = 0;

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
    else if (relativeVel.y > 0)
    {
        entryY = (bT->lastPosition.y - (aT->lastPosition.y + aC->size.y)) / relativeVel.y;
        exitY  = (bT->lastPosition.y + bC->size.y - aT->lastPosition.y) / relativeVel.y;
    }
    else
    {
        entryY = (bT->lastPosition.y + bC->size.y - aT->lastPosition.y) / relativeVel.y;
        exitY  = (bT->lastPosition.y - (aT->lastPosition.y + aC->size.y)) / relativeVel.y;
    }

    float entryTime = std::max(entryX, entryY);
    float exitTime  = std::min(exitX, exitY);

    if (entryTime > 1.f || exitTime < 0 || entryTime > exitTime)
        return false;

    float aCenterX = aT->currentPosition.x + (aT->currentSize.x * 0.5f);
    float aCenterY = aT->currentPosition.y + (aT->currentSize.y * 0.5f);

    float bCenterX = bT->currentPosition.x + (bT->currentSize.x * 0.5f);
    float bCenterY = bT->currentPosition.y + (bT->currentSize.y * 0.5f);

    if (entryX > entryY)
    {
        normal.x = (aCenterX < bCenterX) ? -1 : 1;
    }
    else
    {
        normal.y = (aCenterY < bCenterY) ? -1 : 1;
    }

    if (entryTime < 0.f) entryTime = 0.f;
    timeOfCollision = entryTime;

    return true;
}

void HandleEnterStay(World& world, uint8_t a, uint8_t b, float toi, Vector2 normal)
{
    auto& col = world.colliders[a];
    auto& response = world.collisionResponses[a];

    int slot = FindCollisionSlot(col, b);

    CollisionInfo info{ toi, normal };

    if (slot >= 0)
    {
        col.currentCollisions[slot].info = info;

        if (response.OnStay)
            response.OnStay(world, a, b);
    }
    else
    {
        AddCollision(col, b, info);

        if (response.OnEnter)
            response.OnEnter(world, a, b);
    }
}

void HandleExit(World& world, uint8_t a, uint8_t b)
{
    auto& col = world.colliders[a];
    auto& response = world.collisionResponses[a];

    int slot = FindCollisionSlot(col, b);

    if (slot >= 0)
    {
        col.currentCollisions[slot].otherID = UINT8_MAX;

        if (response.OnExit)
            response.OnExit(world, a, b);
    }
}

void CollisionSystem::Update(World& world)
{
    uint32_t requiredMask = TransformBit | ColliderBit;

    CleanupCollisions(world);

    for (uint8_t i = 0; i < MAX_ENTITIES; i++)
    {
        if ((world.entities[i].mask & requiredMask) != requiredMask) continue;

        auto& transformA = world.transforms[i];
        auto& colliderA  = world.colliders[i];

        for (uint8_t j = i + 1; j < MAX_ENTITIES; j++)
        {
            if ((world.entities[j].mask & requiredMask) != requiredMask) continue;

            auto& transformB = world.transforms[j];
            auto& colliderB  = world.colliders[j];

            float toi = 0.f;
            Vector2 normal{0, 0};

            bool hit = SweptAABB(
                &transformA, &colliderA,
                &transformB, &colliderB,
                toi, normal
            );

            if (hit)
            {
                HandleEnterStay(world, i, j, toi, normal);
                HandleEnterStay(world, j, i, toi, {-normal.x, -normal.y});
            }
            else
            {
                HandleExit(world, i, j);
                HandleExit(world, j, i);
            }
        }
    }
}