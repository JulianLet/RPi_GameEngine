#include "UIRenderSystem.h"

#include "Managers/Game/World.h"
#include "Hardware/Renderer.h"

void UIRenderSystem::Render(World& world, Renderer &renderer)
{
    for (uint8_t e = 0; e < MAX_ENTITIES; e++)
    {
        if (!world.entities[e].isAlive) continue;
        if (!world.entities[e].mask & TransformBit) continue;
        if (world.entities[e].tag != EntityTag::UI) continue;

        if (world.entities[e].mask & UITextBit)
        {
            auto& transform = world.transforms[e];
            auto& uiText = world.uiTexts[e];

            renderer.DrawText((int)transform.currentPosition.x, (int)transform.currentPosition.y, uiText.text.c_str(), uiText.textColor);
        }

        if (world.entities[e].mask & RectangleBit)
        {
            auto& transform = world.transforms[e];
            auto& rec = world.rectangles[e];

            renderer.DrawRectangle(
                (int)transform.currentPosition.x,
                (int)transform.currentPosition.y, 
                (int)transform.currentSize.x,
                (int)transform.currentSize.y,
                rec.currentColor,
                rec.filled
            );
        }
    }
}