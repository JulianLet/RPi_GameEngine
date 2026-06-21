#pragma once

#include "Common.hpp"

struct World;
class TransformComponent;

class PhysicsSystem
{
    public:
    PhysicsSystem() = default;
    ~PhysicsSystem() = default;

    void Update(World& world, float deltaTime);
    void ResolveCollisions(World& world, float deltaTime);

    private:
    void CollisionOneDynamic(Entity* dynamic, Entity* other, float& timeOfCollision, Vector2& normal, float deltaTime);
    void CollisionKinematicStatic(Entity* dynamic, Entity* other, float& timeOfCollision);
    void CollisionDynamicDynamic(Entity* a, Entity* b, float& timeOfCollision, Vector2& normal, float deltaTime);
    Vector2 GetOverlaps(TransformComponent* self, TransformComponent* other);
};