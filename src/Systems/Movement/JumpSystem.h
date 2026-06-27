#pragma once

struct World;

class JumpSystem
{
    public:
    JumpSystem() = default;
    ~JumpSystem() = default;

    void Update(World& world, float deltaTime);
};