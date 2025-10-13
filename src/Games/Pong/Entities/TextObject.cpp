#include "TextObject.h"

#include "Entities/Components/Core/TransformComponent.h"
#include "Entities/Components/UI/UITextComponent.h"

TextObject::TextObject(Vector2 position, const char* text, uint16_t textColor)
{
    AddComponent<TransformComponent>(position, Vector2{0,0});
    AddComponent<UITextComponent>(text, textColor);
}