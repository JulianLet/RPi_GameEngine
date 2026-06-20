#pragma once

#include <functional>

class Entity;

struct CollisionResponseComponent
{
    std::function<void(Entity* self, Entity* other)> OnEnter;
    std::function<void(Entity* self, Entity* other)> OnStay;
    std::function<void(Entity* self, Entity* other)> OnExit;
};