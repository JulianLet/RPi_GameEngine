#pragma once

#include <vector>
#include <memory>

class Entity;

class MenuActionSystem
{

    public:
    MenuActionSystem();
    ~MenuActionSystem() = default;

    void Update(const std::vector<std::unique_ptr<Entity>>& entities);
};