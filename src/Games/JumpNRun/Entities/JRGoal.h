#pragma once

#include "Entities/Entity.h"

class JRGoal : public Entity
{
    void OnCollisionEnter(Entity* self, Entity* other);

    public:
    JRGoal(Vector2 pos, Vector2 size);
    ~JRGoal() = default;
};