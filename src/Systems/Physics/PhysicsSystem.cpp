#include "PhysicsSystem.h"

#include "Managers/Game/World.h"

#include <unordered_set>
#include <cmath>
#include <algorithm>

inline int sign(float v) { return (v > 0.f) - (v < 0.f); }

Vector2 GetOverlaps(const TransformComponent& a, const TransformComponent& b)
{
    float aL = a.currentPosition.x;
    float aR = a.currentPosition.x + a.currentSize.x;
    float aT = a.currentPosition.y;
    float aB = a.currentPosition.y + a.currentSize.y;

    float bL = b.currentPosition.x;
    float bR = b.currentPosition.x + b.currentSize.x;
    float bT = b.currentPosition.y;
    float bB = b.currentPosition.y + b.currentSize.y;

    float ox = std::min(aR, bR) - std::max(aL, bL);
    float oy = std::min(aB, bB) - std::max(aT, bT);

    if (ox < 0) ox = 0;
    if (oy < 0) oy = 0;

    return { ox, oy };
}

void CollisionOneDynamic(World& world, uint32_t a, uint32_t b, float toi, Vector2 normal, float dt)
{
    auto& ta = world.transforms[a];
    auto& pa = world.physics[a];

    auto& tb = world.transforms[b];

    float bounce = 0.0f;

    bounce += world.physicsMaterials[a].bounciness;
    bounce += world.physicsMaterials[b].bounciness;
    bounce *= 0.5f;

    ta.currentPosition = ta.lastPosition + pa.currentVelocity * toi * dt;

    Vector2 v = pa.currentVelocity;
    Vector2 nComp = normal * (v.x * normal.x + v.y * normal.y);
    Vector2 tComp = v - nComp;

    pa.currentVelocity = tComp - nComp * bounce;

    ta.currentPosition += pa.currentVelocity * (1.0f - toi) * dt;
    ta.currentPosition -= normal * 0.1f;
}

void CollisionKinematicStatic(World& world, uint32_t k, uint32_t s)
{
    auto& kt = world.transforms[k];
    auto& kp = world.physics[k];
    auto& st = world.transforms[s];

    Vector2 overlap = GetOverlaps(kt, st);

    if (overlap.x <= 0 && overlap.y <= 0) return;

    if (overlap.x < overlap.y)
    {
        int dir = (kp.currentVelocity.x < 0) ? 1 : -1;
        kt.currentPosition.x += dir * overlap.x;
    }
    else
    {
        int dir = (kp.currentVelocity.y < 0) ? 1 : -1;
        kt.currentPosition.y += dir * overlap.y;
    }
}

void CollisionDynamicDynamic(World& world, uint32_t a, uint32_t b, float toi, Vector2 normal, float dt)
{
    auto& ta = world.transforms[a];
    auto& tb = world.transforms[b];

    auto& pa = world.physics[a];
    auto& pb = world.physics[b];

    float bounce = 0.5f * (
        world.physicsMaterials[a].bounciness +
        world.physicsMaterials[b].bounciness
    );

    ta.currentPosition = ta.lastPosition + pa.currentVelocity * toi * dt;
    tb.currentPosition = tb.lastPosition + pb.currentVelocity * toi * dt;

    if (normal.x != 0)
    {
        float va = pa.currentVelocity.x;
        float vb = pb.currentVelocity.x;

        pa.currentVelocity.x = ((1 - bounce) * va + (1 + bounce) * vb) * 0.5f;
        pb.currentVelocity.x = ((1 - bounce) * vb + (1 + bounce) * va) * 0.5f;
    }

    if (normal.y != 0)
    {
        float va = pa.currentVelocity.y;
        float vb = pb.currentVelocity.y;

        pa.currentVelocity.y = ((1 - bounce) * va + (1 + bounce) * vb) * 0.5f;
        pb.currentVelocity.y = ((1 - bounce) * vb + (1 + bounce) * va) * 0.5f;
    }

    ta.currentPosition += normal * 0.05f;
    tb.currentPosition -= normal * 0.05f;
}

void PhysicsSystem::Update(World& world, float dt)
{
    for (uint32_t i = 0; i < MAX_ENTITIES; i++)
    {
        if (!(world.entities[i].mask & (TransformBit | PhysicsBit))) continue;

        auto& t = world.transforms[i];
        auto& p = world.physics[i];

        if (p.physicsType == PhysicsType::STATIC)
        {
            p.currentVelocity = {0, 0};
            continue;
        }

        if (p.useGravity)
        {
            p.currentVelocity.y += 300.0f * dt;
        }

        t.currentPosition.x += p.currentVelocity.x * dt;
        t.currentPosition.y += p.currentVelocity.y * dt;
    }
}

void PhysicsSystem::ResolveCollisions(World& world, float dt)
{
    std::unordered_set<uint32_t> checked;

    for (uint32_t a = 0; a < MAX_ENTITIES; a++)
    {
        if (!(world.entities[a].mask & ColliderBit)) continue;

        checked.insert(a);

        auto& ca = world.colliders[a];
        auto& pa = world.physics[a];

        for (auto& entry : ca.currentCollisions)
        {
            uint32_t b = entry.otherID;

            if (checked.count(b)) continue;

            auto& cb = world.colliders[b];
            auto& pb = world.physics[b];

            if (!ca.isTrigger && !cb.isTrigger)
            {
                if (pa.physicsType == PhysicsType::STATIC &&
                    pb.physicsType == PhysicsType::STATIC)
                    continue;

                if (pa.physicsType == PhysicsType::KINEMATIC &&
                    pb.physicsType == PhysicsType::STATIC)
                {
                    CollisionKinematicStatic(world, a, b);
                }
                else if (pa.physicsType == PhysicsType::STATIC &&
                         pb.physicsType == PhysicsType::KINEMATIC)
                {
                    CollisionKinematicStatic(world, b, a);
                }
                else if (pa.physicsType == PhysicsType::DYNAMIC &&
                         pb.physicsType == PhysicsType::DYNAMIC)
                {
                    CollisionDynamicDynamic(world, a, b,
                        entry.info.timeOfCollision,
                        entry.info.collisionNormal,
                        dt);
                }
                else if (pa.physicsType == PhysicsType::DYNAMIC)
                {
                    CollisionOneDynamic(world, a, b,
                        entry.info.timeOfCollision,
                        entry.info.collisionNormal,
                        dt);
                }
                else if (pb.physicsType == PhysicsType::DYNAMIC)
                {
                    CollisionOneDynamic(world, b, a,
                        entry.info.timeOfCollision,
                        entry.info.collisionNormal,
                        dt);
                }
            }
        }
    }
}