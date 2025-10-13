#include "UIButtonObject.h"

#include "Entities/Components/Core/TransformComponent.h"
#include "Entities/Components/Render/RectangleComponent.h"
#include "Entities/Components/Render/RenderableComponent.h"
#include "Entities/Components/UI/UIButtonComponent.h"

UIButtonObject::UIButtonObject(Vector2 pos, Vector2 size, uint16_t baseColor, uint16_t selectedColor, int layer)
{
    AddComponent<TransformComponent>(pos, size);
    AddComponent<RectangleComponent>(baseColor, true);
    AddComponent<UIButtonComponent>(baseColor, selectedColor);
    AddComponent<RenderableComponent>(layer);
}