#include "Games/Pong/Entities/PongManager.h"

#include "Hardware/Input.h"

#include "Entities/Components/InputMapping.h"
#include "Entities/Components/InputIntend.h"
#include "Entities/Components/OnEventComponent.h"
#include "PongManager.h"

#include "Systems/EventSystem.h"
#include "Events/Event.h"

PongManager::PongManager()
{
    AddComponent<InputIntendComponent>();
    AddComponent<InputMappingComponent>();

    auto* mapping = GetComponent<InputMappingComponent>();
    mapping->actionMapping[KEYCODE::A] = {InputAction::START_GAME};

    AddComponent<OnEventComponent>();

    auto* onEvent = GetComponent<OnEventComponent>();
    onEvent->events[EventType::BALL_OOB] = [this](const Event &event) 
    {
        this->BallOutOfBounds(event);
    };
}

void PongManager::BallOutOfBounds(const Event &event)
{
    const EventBallOOB* e = static_cast<const EventBallOOB*>(&event);

    if (e->side < 0) scorePlayerTwo++;
    else scorePlayerOne++;

    EventUpdateScore ev(scorePlayerOne, scorePlayerTwo);
    EventSystem::GetInstance().DispatchEvent(ev);
}