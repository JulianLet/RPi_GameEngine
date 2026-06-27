#include "CommonUIFactory.h"

#include "Managers/Game/World.h"

uint8_t CommonUIFactory::CreateUIButton(World &world, Vector2 pos, Vector2 size, uint8_t baseColor, uint8_t selectedColor, int layer)
{
    uint8_t id = world.CreateEntity();

    if (id == INVALID_ENTITY) return INVALID_ENTITY;

    world.entities[id].mask = TransformBit | RectangleBit | RenderableBit | UIButtonBit;

    world.transforms[id] =
    {
        .currentPosition = pos,
        .lastPosition = pos,
        .currentSize = size
    };

    world.rectangles[id] =
    {
        .currentColor = baseColor,
        .filled = FILLED
    };

    world.renderables[id] =
    {
        .doRender = true,
        .layer = layer,
        .parallaxFactor = 1.0f
    };

    world.uiButtons[id] =
    {
        .defaultColor = baseColor,
        .selectedColor = selectedColor
    };

    return id;
}

uint8_t CommonUIFactory::CreateUIText(World &world, Vector2 pos, const char* text, uint8_t textColor, int layer)
{
    uint8_t id = world.CreateEntity();

    if (id == INVALID_ENTITY) return INVALID_ENTITY;

    world.entities[id].mask = TransformBit | RenderableBit | UITextBit;

    world.transforms[id] = 
    {
        .currentPosition = pos,
        .lastPosition = pos,
        .currentSize = Vector2(0,0)
    };

    world.renderables[id] =
    {
        .doRender = true,
        .layer = layer,
        .parallaxFactor = 1.0f
    };

    world.uiTexts[id] =
    {
        .text = text,
        .textColor = textColor
    };

    return id;
}

uint8_t CommonUIFactory::CreateTimerText(World &world, Vector2 pos, uint8_t color, int layer)
{
    uint8_t id = world.CreateEntity();

    if (id == INVALID_ENTITY) return INVALID_ENTITY;

    world.entities[id].mask = TransformBit | RenderableBit | TimerBit | UITextBit;

    world.transforms[id] = 
    {
        .currentPosition = pos,
        .lastPosition = pos,
        .currentSize = Vector2(0,0)
    };

    world.renderables[id] =
    {
        .doRender = true,
        .layer = layer,
        .parallaxFactor = 1.0f
    };

    world.timers[id] =
    {
        .currentTime = 0
    };

    world.uiTexts[id] =
    {
        .text = "start",
        .textColor = color
    };

    return id;
}
