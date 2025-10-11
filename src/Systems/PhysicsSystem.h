#pragma once

#include "Common.h"
#include <vector>
#include <memory>

class Entity;
class TransformComponent;

class PhysicsSystem
{
    public:
    PhysicsSystem() = default;
    ~PhysicsSystem() = default;

    void Update(const std::vector<std::unique_ptr<Entity>>& entities, float deltaTime);
    void ResolveCollisions(const std::vector<std::unique_ptr<Entity>>& entities);

    private:
    void CollisionOneDynamic(Entity* dynamic, Entity* other);
    void CollisionKinematicStatic(Entity* dynamic, Entity* other);
    void CollisionDynamicDynamic(Entity* self, Entity* other);
    Vector2 GetOverlaps(TransformComponent* self, TransformComponent* other);
};