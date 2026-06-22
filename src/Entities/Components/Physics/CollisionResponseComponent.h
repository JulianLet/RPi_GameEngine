#pragma once

#include <functional>

class Entity;

struct CollisionResponseComponent
{
    std::function<void(uint32_t self, uint32_t other)> OnEnter;
    std::function<void(uint32_t self, uint32_t other)> OnStay;
    std::function<void(uint32_t self, uint32_t other)> OnExit;
};