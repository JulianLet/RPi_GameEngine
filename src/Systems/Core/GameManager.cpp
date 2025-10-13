#include "GameManager.h"

#include "Systems/Events/EventSystem.h"

#include "Games/Pong/Pong.h"
#include "Games/JumpNRun/JumpNRun.h"
#include "Games/Menu/Menu.h"

GameManager::GameManager()
{
    // currentGame = std::make_unique<Pong>(*this);
    // currentGame = std::make_unique<JumpNRun>(*this);
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
}

void GameManager::Render(Renderer& renderer)
{
    currentGame->Render(renderer);
}

void GameManager::SwitchGame(std::unique_ptr<Game> nextGame)
{
    //start threat with animation??
    pendingGame = std::move(nextGame);
}
