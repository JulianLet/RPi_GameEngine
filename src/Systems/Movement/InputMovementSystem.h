#pragma once

struct World;

class InputMovementSystem
{
    public:
    InputMovementSystem() = default;
    ~InputMovementSystem() = default;

    void Update(World& world, float deltaTime);
};