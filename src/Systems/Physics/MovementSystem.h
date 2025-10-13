#pragma once

#include <memory>
#include <vector>

class Entity;

class MovementSystem
{


    public:
    MovementSystem() = default;
    ~MovementSystem() = default;

    void Update(const std::vector<std::unique_ptr<Entity>>& entities, float deltaTime);
};