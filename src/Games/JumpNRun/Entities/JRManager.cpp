#include "JRManager.h"

#include "Entities/Components/Events/OnEventComponent.h"
#include "Entities/Components/Input/InputMappingComponent.h"
#include "Entities/Components/Input/InputIntendComponent.h"

void JRManager::CheckHighScore(float thisTime)
{
    if (thisTime < bestTime) 
    {
        bestTime = thisTime;
    }
}

JRManager::JRManager()
{
    AddComponent<OnEventComponent>();
    auto* onEvent = GetComponent<OnEventComponent>();
    onEvent->events[EventType::UPDATE_SCORE] = [this](const Event &event) 
    {
        const EventUpdateScore* e = static_cast<const EventUpdateScore*>(&event);

        this->CheckHighScore(e->endTime);
    };

    AddComponent<InputIntendComponent>();

    //add inputs mapping
    AddComponent<InputMappingComponent>();
    auto* mapping = GetComponent<InputMappingComponent>();
    mapping->actionMapping[KEYCODE::A] = {InputAction::START_GAME};
}