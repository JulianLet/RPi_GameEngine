#pragma once

class Input;
struct World;

class TimerSystem
{
    public:
    TimerSystem() = default;
    ~TimerSystem() = default;

    void Update(World& world, float deltaTime);
};