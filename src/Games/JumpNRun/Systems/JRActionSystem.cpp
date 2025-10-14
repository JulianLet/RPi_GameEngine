#include "JRActionSystem.h"

#include "Games/JumpNRun/JumpNRun.h"
#include "Games/JumpNRun/Entities/JRManager.h"
#include "Systems/Events/EventSystem.h"
#include "Systems/Events/Event.h"

#include "Entities/Entity.h"
#include "Entities/Components/Input/InputIntendComponent.h"

#include <string>

JRActionSystem::JRActionSystem(JumpNRun* jrRef, JRManager* manager) 
    : jrRef(jrRef), manager(manager)
{
    EventSystem::GetInstance().AddListener(this, jrRef);
};

void JRActionSystem::Update(const std::vector<std::unique_ptr<Entity>>& entities, float deltaTime)
{
    for (auto& entity : entities)
    {
        InputIntendComponent* intend = entity->GetComponent<InputIntendComponent>();
        if (!intend) continue;

        if (intend->actions[InputAction::JUMP])
        {
            EventJump e;
            EventSystem::GetInstance().DispatchEvent(e);
        }

        if (intend->actions[InputAction::START_GAME])
        {
            EventStartGame e;
            EventSystem::GetInstance().DispatchEvent(e);
        }
    }
}

void JRActionSystem::HandleEvent(const Event &event)
{
    if (event.GetEventType() == EventType::START_GAME)
    {
        jrRef->StartGame();
    }

    if (event.GetEventType() == EventType::UPDATE_SCORE)
    {
        jrRef->ResetGame();
    }
}