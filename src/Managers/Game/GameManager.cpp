#include "GameManager.h"

#include "Hardware/Input.h"
#include "Managers/Event/EventManager.h"
#include "Managers/Core/DebugManager.h"

#include "Games/Menu/Menu.h"

GameManager::GameManager()
{
    currentGame = std::make_unique<Menu>(*this);
    DebugManager::GetInstance().printLogs = true;
}

void GameManager::Update(Input& input, float deltaTime)
{
    if (pendingGame) //switch game before anything gets updated
    {
        EventManager::GetInstance().RemoveListenersForGame(currentGame.get());
        currentGame = std::move(pendingGame);
    }

    currentGame->Update(input, deltaTime);

    if (input.GetKey(KEYCODE::START).pressed)
    {
        SwitchGame(std::make_unique<Menu>(*this));
    }

    if (input.GetKey(KEYCODE::B).pressed)
    {
        DebugManager::GetInstance().printLogs = !DebugManager::GetInstance().printLogs;
    }

    std::string msg = "DT: " + std::to_string(deltaTime);
    DebugManager::GetInstance().Log(msg.c_str());

    //delat times after spi split    
    //menu: 0.016 (60 FPS)
    //pong: 0.017 (59 FPS)
    //jnr:  0.018 (55 FPS)
    //sd:   0.046 (22 FPS) (no animations)


    //delta times when saving the sprite and drawing on framebuffer
    //menu: 0,05 (20 FPS)
    //pong: 0,05 (20 FPS)
    //jnr:  0,05 (20 FPS)
    //sd:   0,16 ( 6 FPS) (0,14 if not drawing the whole screen)

    //delta times when saving sprite but drawing directly on screen
    //menu: 0,12
    //pong: 0,15
    //jrn:  0,14
    //sd:   0,45
}

void GameManager::Render(Renderer& renderer)
{
    currentGame->Render(renderer);
    DebugManager::GetInstance().Render(renderer);
}

void GameManager::SwitchGame(std::unique_ptr<Game> nextGame)
{
    //start threat with animation??
    pendingGame = std::move(nextGame);
}
