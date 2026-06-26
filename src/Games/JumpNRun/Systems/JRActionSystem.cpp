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

void JRActionSystem::Update(World& world, Input& input)
{
    // if start is pressed, start game
    if (!gameRef->runGame && input.GetKey(KEYCODE::A).pressed)
    {
        gameRef->runGame = true;
    }

    // jump????
}

void JRActionSystem::HandleEvent(const Event &event)
{
    if (event.GetEventType() == EventType::UPDATE_SCORE)
    {
        // check best time
        gameRef->ResetGame();
    }
}