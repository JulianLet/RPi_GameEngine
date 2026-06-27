#include "PongActionSystem.h"

#include "Hardware/Input.h"
#include "Games/Pong/Pong.h"
#include "Managers/Managers.h"

PongActionSystem::PongActionSystem(Pong* pong)
    : gameRef(pong)
{
    scorePlayerOne = 0;
    scorePlayerTwo = 0;
}

void PongActionSystem::Initialize(World& world, uint8_t ballID, uint8_t scoreTextID)
{
    this->ballID = ballID;
    this->scoreTextID = scoreTextID;

    SetText(world);
}

void PongActionSystem::Update(World &world, Input& input)
{
    // check if ball is oob
    auto& ballPos = world.transforms[ballID];

    if (ballPos.currentPosition.x < -5) 
    {
        gameRef->ResetGame();
        UpdateText(world, false);
    }
    else if (ballPos.currentPosition.x > gameRef->playingFieldSize.x + 5) 
    {
        gameRef->ResetGame();
        UpdateText(world, true);
    }

    // if start is pressed, start game
    if (!gameRef->runGame && input.GetKey(KEYCODE::A).pressed)
    {
        gameRef->runGame = true;
    }
}

void PongActionSystem::UpdateText(World& world, bool playerOneScored)
{
    if (playerOneScored) scorePlayerOne++;
    else scorePlayerTwo++;
    
    SetText(world);
}

void PongActionSystem::SetText(World& world)
{
    char buffer[16];
    snprintf(buffer, sizeof(buffer), "%d : %d", scorePlayerOne, scorePlayerTwo);

    world.uiTexts[scoreTextID].text = buffer;
}