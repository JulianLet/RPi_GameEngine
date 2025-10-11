#pragma once

#include <vector>
#include <memory>

class Event;
class Entity;
class Player;

class JRActionSystem
{
    public: 
    JRActionSystem();
    ~JRActionSystem() = default;

    void Update(const std::vector<std::unique_ptr<Entity>> &entities);
};