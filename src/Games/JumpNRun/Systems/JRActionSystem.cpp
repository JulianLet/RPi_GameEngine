#include "JRActionSystem.h"

#include "Systems/EventSystem.h"
#include "Events/Event.h"

#include "Entities/Components/InputIntend.h"
#include "Entities/Entity.h"

JRActionSystem::JRActionSystem() = default;

void JRActionSystem::Update(const std::vector<std::unique_ptr<Entity>>& entities)
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
    }
}