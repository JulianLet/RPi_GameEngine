#include "JRActionSystem.h"

#include "Games/JumpNRun/JumpNRun.h"
#include "Hardware/Input.h"

#include <string>

JRActionSystem::JRActionSystem(JumpNRun* gameRef) 
    : gameRef(gameRef)
{
    currentBestTime = 99999.f;
    EventManager::GetInstance().AddListener(this, gameRef);
};

void JRActionSystem::Initialize(uint8_t playerID)
{
    // display the current best time
    this->playerID = playerID;
}

void JRActionSystem::Update(World &world, Input &input)
{
    // if start is pressed, start game
    if (!gameRef->runGame && input.GetKey(KEYCODE::A).pressed)
    {
        gameRef->runGame = true;
    }

    auto& playerTransform = world.transforms[playerID];
    if (playerTransform.currentPosition.y > 250) gameRef->ResetGame();
}

void JRActionSystem::HandleEvent(const Event &event)
{
    if (event.GetEventType() == EventType::UPDATE_SCORE)
    {
        // check best time in event
        gameRef->ResetGame();
    }
}