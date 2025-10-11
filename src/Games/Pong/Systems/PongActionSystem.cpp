#include "Games/Pong/Systems/PongActionSystem.h"
#include "Systems/EventSystem.h"
#include "Events/Event.h"
#include "Entities/Components/InputIntend.h"
#include "Entities/Entity.h"
#include "Games/Pong/Pong.h"

PongActionSystem::PongActionSystem(Pong* pong)
    : pongRef(pong)
{
    EventSystem::GetInstance().AddListener(this);
}

void PongActionSystem::Update(const std::vector<std::unique_ptr<Entity>>& entities, Pong& pongRef)
{
    for (auto& entity : entities)
    {
        InputIntendComponent* intend = entity->GetComponent<InputIntendComponent>();
        if (!intend) continue;

        if (intend->actions[InputAction::START_GAME])
        {
            EventStartGame e;
            EventSystem::GetInstance().DispatchEvent(e);
        }
    }
}

void PongActionSystem::HandleEvent(const Event& event)
{
    if (event.GetEventType() == EventType::START_GAME)
    {
        pongRef->runGame = true;
    }
}
