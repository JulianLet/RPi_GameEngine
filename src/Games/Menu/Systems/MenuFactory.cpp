#include "MenuFactory.h"

#include "Common.hpp"

void MenuFactory::CreateMenuButton(World& world, Vector2 pos, const char* text, std::function<void(GameManager&)> onClick)
{
    auto button = myUIFactory.CreateUIButton(
        world,
        pos,
        Vector2(110, 20),
        Color::ORANGE,
        Color::LIGHTGRAY,
        0
    );

    world.uiButtons[button].OnClicked = std::move(onClick);

    myUIFactory.CreateUIText(world, Vector2(pos.x + 20, pos.y + 7), text, Color::BLACK, -1);
}