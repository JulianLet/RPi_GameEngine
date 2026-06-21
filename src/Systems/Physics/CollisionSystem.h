#pragma once

#include "Common.hpp"

struct World;
struct TransformComponent;
struct ColliderComponent;

class CollisionSystem
{
public:
    CollisionSystem() = default;
    ~CollisionSystem() = default;
    
    void Update(World& world);

private:
    bool SweptAABB(TransformComponent* aT, ColliderComponent* aC, TransformComponent* bT, ColliderComponent* bC, float& timeOfCollision, Vector2& normal);

    void HandleEnterStayPos(Entity* self, ColliderComponent* collider, Entity* other, float& timeOfCollision, Vector2& normal);
    void HandleExitPos(Entity* self, ColliderComponent* collider, Entity* other);
    Vector2 GetOverlaps(TransformComponent* self, TransformComponent* other);
};