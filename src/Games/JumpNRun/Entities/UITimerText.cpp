#include "UITimerText.h"

#include "Entities/Components/Core/TimerComponent.h"
#include "Entities/Components/Core/TransformComponent.h"
#include "Entities/Components/Render/RenderableComponent.h"
#include "Entities/Components/UI/UITextComponent.h"

UITimerText::UITimerText(Vector2 position, uint16_t textColor, int layer)
{
    AddComponent<TimerComponent>();
    AddComponent<TransformComponent>(position, Vector2{0,0});
    AddComponent<UITextComponent>("0", textColor);
    AddComponent<RenderableComponent>(layer);
}