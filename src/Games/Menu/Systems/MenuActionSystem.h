#pragma once

#include <vector>
#include <memory>

class World;

class MenuActionSystem
{
    public:
    MenuActionSystem();
    ~MenuActionSystem() = default;

    void Update(World& world);
};