#pragma once

struct World;
class Input;
class GameManager;

class UIButtonSystem
{
    int currentSelected = 0;

    public:
    UIButtonSystem() = default;
    ~UIButtonSystem() = default;

    void Initialize(World& world);
    void Update(World& world, Input& input, GameManager& gameManager);
    void UpdateVisuals(World& world);
    void PressButton(World& world, GameManager& manager);
};