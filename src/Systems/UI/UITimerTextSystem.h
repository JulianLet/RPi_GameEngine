#pragma once

struct World;
class Input;
class GameManager;

class UITimerTextSystem
{
    int currentSelected = 0;

    public:
    UITimerTextSystem() = default;
    ~UITimerTextSystem() = default;

    void Update(World& world);
};