#include "UIButtonSystem.h"

#include "Managers/Managers.h"

uint8_t FindNextButton(World& world, uint8_t current)
{
    // Handle invalid selection.
    if (current >= MAX_ENTITIES)
    {
        for (uint8_t e = 0; e < MAX_ENTITIES; ++e)
        {
            if (world.entities[e].mask & UIButtonBit)
            {
                return e;
            }
        }

        return INVALID_ENTITY;
    }

    uint8_t idx = current;

    do
    {
        idx = (idx + 1) % MAX_ENTITIES;

        if (world.entities[idx].mask & UIButtonBit)
        {
            return idx;
        }

    } while (idx != current);

    return current;
}

uint8_t FindPreviousButton(World& world, uint8_t current)
{
    // Handle invalid selection.
    if (current >= MAX_ENTITIES)
    {
        for (uint8_t e = MAX_ENTITIES; e > 0; --e)
        {
            uint8_t idx = e - 1;

            if (world.entities[idx].mask & UIButtonBit)
            {
                return idx;
            }
        }

        return INVALID_ENTITY;
    }

    uint8_t idx = current;

    do
    {
        idx = (idx == 0) ? MAX_ENTITIES - 1 : idx - 1;

        if (world.entities[idx].mask & UIButtonBit)
        {
            return idx;
        }

    } while (idx != current);

    return current;
}

void UIButtonSystem::Initialize(World& world)
{
    for (uint8_t e = 0; e < MAX_ENTITIES; e++)
    {
        if (world.entities[e].mask & UIButtonBit)
        {
            currentSelected = e;
            return;
        }
    }

    currentSelected = INVALID_ENTITY;
}

void UIButtonSystem::Update(World& world, Input& input, GameManager& gameManager)
{
    if (input.GetKey(KEYCODE::UP).pressed)
    {
        currentSelected = FindPreviousButton(world, currentSelected);
    }

    if (input.GetKey(KEYCODE::DOWN).pressed)
    {
        currentSelected = FindNextButton(world, currentSelected);
    }

    UpdateVisuals(world);

    if (input.GetKey(KEYCODE::A).pressed)
    {
        PressButton(world, gameManager);
    }
}

void UIButtonSystem::UpdateVisuals(World& world)
{
    uint32_t requiredMask = RectangleBit | UIButtonBit;

    for (uint8_t e = 0; e < MAX_ENTITIES; e++)
    {
        if ((world.entities[e].mask & requiredMask) != requiredMask)
            continue;

        auto& rec = world.rectangles[e];
        auto& btn = world.uiButtons[e];

        rec.currentColor =
            (e == currentSelected)
                ? btn.selectedColor
                : btn.defaultColor;
    }
}

void UIButtonSystem::PressButton(World& world, GameManager& manager)
{
    if (currentSelected >= MAX_ENTITIES)
        return;

    if (!(world.entities[currentSelected].mask & UIButtonBit))
        return;

    auto& btn = world.uiButtons[currentSelected];

    if (btn.OnClicked)
    {
        btn.OnClicked(manager);
    }
}