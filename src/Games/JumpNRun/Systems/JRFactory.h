#pragma once

#include "Entities/Common/CommonFactory.h"
#include "Entities/Common/CommonUIFactory.h"
#include "Managers/Managers.h"

struct JRFactory
{
    CommonFactory myFactory;
    CommonUIFactory myUIFactory;

    uint8_t CreateCloud(World& world, Vector2 pos, Vector2 size);
    uint8_t CreateGoal(World& world, Vector2 pos, Vector2 size);
};