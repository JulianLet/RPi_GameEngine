#include "JRManager.h"

#include "Entities/Components/Events/OnEventComponent.h"
#include "Entities/Components/Input/InputMappingComponent.h"
#include "Entities/Components/Input/InputIntendComponent.h"

void JRManager::CheckHighScore()
{
    if (currentTime < bestTime) 
    {
        bestTime = currentTime;
    }
}

JRManager::JRManager()
{
    AddComponent<OnEventComponent>();
    auto* onEvent = GetComponent<OnEventComponent>();
    onEvent->events[EventType::UPDATE_SCORE] = [this](const Event &event) 
    {
        this->CheckHighScore();
    };

    AddComponent<InputIntendComponent>();

    //add inputs mapping
    AddComponent<InputMappingComponent>();
    auto* mapping = GetComponent<InputMappingComponent>();
    mapping->actionMapping[KEYCODE::A] = {InputAction::START_GAME};
}