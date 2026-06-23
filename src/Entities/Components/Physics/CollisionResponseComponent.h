#pragma once

#include <cstdint>
#include <functional>

struct World;

struct CollisionResponseComponent
{
    std::function<void(World& world, uint8_t self, uint8_t other)> OnEnter;
    std::function<void(World& world, uint8_t self, uint8_t other)> OnStay;
    std::function<void(World& world, uint8_t self, uint8_t other)> OnExit;
};