#pragma once

#include "Entities/Component.h"

#include <functional>

class Entity;

struct CollisionResponseComponent : public Component
{
    std::function<void(Entity* self, Entity* other)> OnEnter;
    std::function<void(Entity* self, Entity* other)> OnStay;
    std::function<void(Entity* self, Entity* other)> OnExit;

    CollisionResponseComponent() = default;
    ~CollisionResponseComponent() override;

    void Reset() override;
};