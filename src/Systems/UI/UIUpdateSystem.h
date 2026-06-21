#pragma once

struct World;
class Input;
class GameManager;

class UIUpdateSystem
{
    int currentSelected = 0;

    public:
    UIUpdateSystem() = default;
    ~UIUpdateSystem() = default;

    void Update(World& world, Input& input, GameManager& gameManager);
};