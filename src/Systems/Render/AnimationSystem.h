#pragma once

#include <vector>
#include <memory>

class Entity;

class AnimationSystem
{

    public:
    AnimationSystem() = default;
    ~AnimationSystem() = default;

    void Update(const std::vector<std::unique_ptr<Entity>>& entities, float deltaTime);
};