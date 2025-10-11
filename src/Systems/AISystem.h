#pragma once

#include <vector>
#include <memory>

class Entity;

class AISystem
{
    public:
    AISystem() = default;
    ~AISystem() = default;
    
    void Update(const std::vector<std::unique_ptr<Entity>>& entities, float deltaTime);
};