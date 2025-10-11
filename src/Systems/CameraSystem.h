#pragma once

#include <vector>
#include <memory>

class Entity;

class CameraSystem
{

    public:
    CameraSystem() = default;
    ~CameraSystem() = default;

    void Update(const std::vector<std::unique_ptr<Entity>>& entities, float deltaTime);
};