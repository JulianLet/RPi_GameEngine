#include "GameManager.h"

#include "Hardware/Input.h"
#include "Systems/Events/EventSystem.h"
#include "Systems/Debug/DebugManager.h"

#include "Games/Menu/Menu.h"

GameManager::GameManager()
{
    currentGame = std::make_unique<Menu>(*this);
}

void GameManager::Update(Input& input, float deltaTime)
{
    if (pendingGame) //switch game before anything gets updated
    {
        EventSystem::GetInstance().RemoveListenersForGame(currentGame.get());
        currentGame = std::move(pendingGame);
    }

    currentGame->Update(input, deltaTime);

    if (input.GetKey(KEYCODE::START).pressed)
    {
        SwitchGame(std::make_unique<Menu>(*this));
    }
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
