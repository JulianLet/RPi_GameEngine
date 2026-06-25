#pragma once

#include "Entities/Common/CommonFactory.h"
#include "Entities/Common/CommonUIFactory.h"
#include "Managers/Managers.h"

#include <functional>

namespace PongAI
{
    enum Momory {BALL_ID = 0, ESTEMATED_Y, BALL_TO_RIGHT, TARGET_DIR_Y};
}

struct PongFactory
{
    CommonFactory myFactory;
    CommonUIFactory myUIFactory;

    uint8_t CreatePaddle(World& world, Vector2 startPos, KEYCODE up, KEYCODE down, bool isPlayer, uint8_t ballID);
    uint8_t CreateBall(World& world);

    private:
    //AI functions
    void PaddleThink(World& world, uint8_t self, float deltaTime);
    void PaddleDecide(World& world, uint8_t self, float deltaTime);
    void PaddleAct(World& world, uint8_t self, float deltaTime);
};