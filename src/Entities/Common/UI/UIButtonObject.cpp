#include "UIButtonObject.h"

#include "Entities/Components/Core/TransformComponent.h"
#include "Entities/Components/Render/RectangleComponent.h"
#include "Entities/Components/Render/RenderableComponent.h"
#include "Entities/Components/UI/UIButtonComponent.h"

UIButtonObject::UIButtonObject(Vector2 pos, Vector2 size, uint16_t baseColor, uint16_t selectedColor, int layer)
{
    auto pongButton = std::make_unique<Entity>();
    pongButton->AddComponent<TransformComponent>(pos, size);
    pongButton->AddComponent<RectangleComponent>(baseColor, true);
    pongButton->AddComponent<UIButtonComponent>(baseColor, selectedColor);
    pongButton->AddComponent<RenderableComponent>(layer);
}