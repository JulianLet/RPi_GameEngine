#include "Systems/GameManager.h"
#include "Games/Pong/Pong.h"
#include "Games/JumpNRun/JumpNRun.h"

GameManager::GameManager()
{
    // currentGame = std::make_unique<Pong>(*this);
    currentGame = std::make_unique<JumpNRun>(*this);
}

void GameManager::Update(Input& input, float deltaTime)
{
    currentGame->Update(input, deltaTime);
}

void GameManager::Render(Renderer& renderer)
{
    currentGame->Render(renderer);
}

void GameManager::SwitchGame(std::unique_ptr<Game> nextGame)
{
    //start threat with animation??
    currentGame = std::move(nextGame);
}
