#pragma once
#include "Hardware/Renderer.h"
#include "Hardware/Input.h"
#include "Games/Game.h"
#include <memory>

class GameManager
{
    std::unique_ptr<Game> currentGame;
    std::unique_ptr<Game> pendingGame;

    public:
    GameManager();
    ~GameManager() = default;
    void Update(Input& input, float deltaTime);
    void Render(Renderer& renderer);
    void SwitchGame(std::unique_ptr<Game> nextGame);
};