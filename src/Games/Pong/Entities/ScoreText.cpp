#include "ScoreText.h"

#include "Systems/Events/Event.h"
#include "Systems/Events/EventSystem.h"

#include "Entities/Components/Core/TransformComponent.h"
#include "Entities/Components/Events/OnEventComponent.h"
#include "Entities/Components/UI/UITextComponent.h"

ScoreText::ScoreText(Vector2 position, uint16_t textColor)
{
    const char* text = "0 : 0";
    centerPosition = position;

    AddComponent<TransformComponent>(CenteredText(text), Vector2());
    AddComponent<UITextComponent>(text, textColor);

    AddComponent<OnEventComponent>();

    auto* onEvent = GetComponent<OnEventComponent>();
    onEvent->events[EventType::UPDATE_SCORE] = [this](const Event &event) 
    {
        this->UpdateScore(event);
    };
}

Vector2 ScoreText::CenteredText(const char* text)
{
    int charsTillMiddle = 0;

    while (text[charsTillMiddle] != ':')
    {
        charsTillMiddle++;
    }

    return Vector2(centerPosition.x - charsTillMiddle * 6, centerPosition.y);
}

void ScoreText::UpdateScore(const Event &event)
{
        const EventUpdateScore* e = static_cast<const EventUpdateScore*>(&event);

        // buffer to hold the text, make sure it's large enough
        snprintf(buffer, sizeof(buffer), "%d : %d", e->playerOne, e->playerTwo);

        GetComponent<UITextComponent>()->text = buffer;
        GetComponent<TransformComponent>()->currentPosition = CenteredText(buffer);
}

