#pragma once

struct World;

class MovementSystem
{
    public:
    MovementSystem() = default;
    ~MovementSystem() = default;

    void Update(World& world, float deltaTime);
};