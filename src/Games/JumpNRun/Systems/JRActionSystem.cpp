#include "JRActionSystem.h"

#include "Systems/Events/EventSystem.h"
#include "Systems/Events/Event.h"

#include "Entities/Entity.h"
#include "Entities/Components/Input/InputIntendComponent.h"

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