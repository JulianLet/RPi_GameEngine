#pragma once

#include <vector>
#include <memory>

class Entity;
class Input;
class GameManager;

class UIUpdateSystem
{
    int currentSelected = 0;

    public:
    UIUpdateSystem() = default;
    ~UIUpdateSystem() = default;

    void Update(const std::vector<std::unique_ptr<Entity>>& entities, Input& input, GameManager& gameManager);
};