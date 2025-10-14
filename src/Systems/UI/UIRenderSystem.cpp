#include "UIRenderSystem.h"

#include "Hardware/Renderer.h"
#include "Entities/Entity.h"

#include "Entities/Components/Core/TransformComponent.h"
#include "Entities/Components/Render/RectangleComponent.h"
#include "Entities/Components/UI/UITextComponent.h"

void UIRenderSystem::Render(const std::vector<std::unique_ptr<Entity>> &entities, Renderer &renderer)
{
    for (auto& entity : entities)
    {
        auto* transform = entity->GetComponent<TransformComponent>();
        if (!transform) continue;


        auto* text = entity->GetComponent<UITextComponent>();
        if (text)
        {
            renderer.DrawText((int)transform->currentPosition.x, (int)transform->currentPosition.y, text->text.c_str(), text->textColor);
            continue;
        }


        auto* rec = entity->GetComponent<RectangleComponent>();
        if (rec)
        {
            renderer.DrawRectangle(
                (int)transform->currentPosition.x,
                (int)transform->currentPosition.y,
                (int)transform->currentSize.x,
                (int)transform->currentSize.y,
                rec->currentColor,
                rec->filled
            );
            continue;
        }
    }
}