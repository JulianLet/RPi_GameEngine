#include "TextObject.h"

#include "Entities/Components/Transform.h"
#include "Entities/Components/UIText.h"

TextObject::TextObject(Vector2 position, const char* text, uint16_t textColor)
{
    AddComponent<TransformComponent>(position, Vector2{0,0});
    AddComponent<UITextComponent>(text, textColor);
}