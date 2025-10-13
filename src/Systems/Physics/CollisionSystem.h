#pragma once

#include "Common.h"
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
    bool SweptAABB(TransformComponent* aT, ColliderComponent* aC, TransformComponent* bT, ColliderComponent* bC, Vector2& intersection);
    bool CheckOverlap(TransformComponent* aT, ColliderComponent* aC, TransformComponent* bT, ColliderComponent* bC);

    void HandleEnterStayPos(Entity* self, ColliderComponent* collider, Entity* other, Vector2& intersection);
    void HandleExitPos(Entity* self, ColliderComponent* collider, Entity* other);
};