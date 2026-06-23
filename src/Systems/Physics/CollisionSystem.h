#pragma once

struct World;

class CollisionSystem
{
public:
    CollisionSystem() = default;
    ~CollisionSystem() = default;
    
    void Update(World& world);
};