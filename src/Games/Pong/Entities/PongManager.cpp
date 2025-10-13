#include "PongManager.h"

#include "Hardware/Input.h"

#include "Systems/Events/Event.h"
#include "Systems/Events/EventSystem.h"

#include "Entities/Components/Input/InputMappingComponent.h"
#include "Entities/Components/Input/InputIntendComponent.h"
#include "Entities/Components/Events/OnEventComponent.h"


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