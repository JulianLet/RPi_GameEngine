#include "UITextObject.h"

#include "Entities/Components/Core/TransformComponent.h"
#include "Entities/Components/Render/RenderableComponent.h"
#include "Entities/Components/UI/UITextComponent.h"

UITextObject::UITextObject(Vector2 position, const char* text, uint16_t textColor, int layer)
{
    AddComponent<TransformComponent>(position, Vector2{0,0});
    AddComponent<UITextComponent>(text, textColor);
    AddComponent<RenderableComponent>(layer);
}