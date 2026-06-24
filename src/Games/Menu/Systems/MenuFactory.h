#pragma once

#include "Entities/Common/CommonFactory.h"
#include "Entities/Common/CommonUIFactory.h"
#include "Managers/Managers.h"

#include <functional>

struct MenuFactory
{
    CommonFactory myFactory;
    CommonUIFactory myUIFactory;

    void CreateMenuButton(World& world, Vector2 pos, const char* text, std::function<void(GameManager&)> onClick);
};