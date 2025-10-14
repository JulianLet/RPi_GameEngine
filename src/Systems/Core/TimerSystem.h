#pragma once

#include <vector>
#include <memory>

class Input;
class Entity;

class TimerSystem
{

    public:
    TimerSystem() = default;
    ~TimerSystem() = default;

    void Update(const std::vector<std::unique_ptr<Entity>>& entities, float deltaTime);
};