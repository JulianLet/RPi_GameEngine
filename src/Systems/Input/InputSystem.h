#pragma once

#include <vector>
#include <memory>

class Input;
class Entity;

class InputSystem
{

    public:
    InputSystem() = default;
    ~InputSystem() = default;

    void Update(const std::vector<std::unique_ptr<Entity>>& entities, Input& input);
};