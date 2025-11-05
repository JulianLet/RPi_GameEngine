#pragma once

#include "Common.hpp"
#include <vector>
#include <memory>

class Entity;
struct TransformComponent;
struct ColliderComponent;

class CollisionSystem
{
public:
    CollisionSystem() = default;
    ~CollisionSystem() = default;
    
    void Update(const std::vector<std::unique_ptr<Entity>>& entities);

private:
    bool SweptAABB(TransformComponent* aT, ColliderComponent* aC, TransformComponent* bT, ColliderComponent* bC, float& timeOfCollision, Vector2& normal);

    void HandleEnterStayPos(Entity* self, ColliderComponent* collider, Entity* other, float& timeOfCollision, Vector2& normal);
    void HandleExitPos(Entity* self, ColliderComponent* collider, Entity* other);
    Vector2 GetOverlaps(TransformComponent* self, TransformComponent* other);
};