#pragma once

#include "Common.hpp"
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
    void ResolveCollisions(const std::vector<std::unique_ptr<Entity>>& entities, float deltaTime);

    private:
    void CollisionOneDynamic(Entity* dynamic, Entity* other, float& timeOfCollision, Vector2& normal, float deltaTime);
    void CollisionKinematicStatic(Entity* dynamic, Entity* other, float& timeOfCollision);
    void CollisionDynamicDynamic(Entity* a, Entity* b, float& timeOfCollision, Vector2& normal, float deltaTime);
    Vector2 GetOverlaps(TransformComponent* self, TransformComponent* other);
};