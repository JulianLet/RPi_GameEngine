#include "UIRenderSystem.h"

#include "Hardware/Renderer.h"
#include "Entities/Entity.h"

#include "Entities/Components/Transform.h"
#include "Entities/Components/UIText.h"

void UIRenderSystem::Render(const std::vector<std::unique_ptr<Entity>> &entities, Renderer &renderer)
{
    for (auto& entity : entities)
    {
        TransformComponent* transform = entity->GetComponent<TransformComponent>();
        UITextComponent* text = entity->GetComponent<UITextComponent>();

        if (text)
        {
            renderer.DrawText((int)transform->currentPosition.x, (int)transform->currentPosition.y, text->text, text->textColor);
        }
    }
}