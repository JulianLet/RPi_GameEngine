#pragma once

struct World;

class FollowMovementSystem
{
    public:
    FollowMovementSystem() = default;
    ~FollowMovementSystem() = default;

    void Update(World& world, float deltaTime);
};