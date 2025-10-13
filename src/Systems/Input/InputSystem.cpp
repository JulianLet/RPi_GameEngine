#include "InputSystem.h"

#include "Systems/Events/Event.h"
#include "Systems/Events/EventSystem.h"

#include "Hardware/Input.h"

#include "Entities/Entity.h"
#include "Entities/Components/Input/InputMappingComponent.h"
#include "Entities/Components/Input/InputIntendComponent.h"



void InputSystem::Update(const std::vector<std::unique_ptr<Entity>> &entities, Input &input)
{
    for (auto& entity : entities)
    {
        InputMappingComponent* mapping = entity->GetComponent<InputMappingComponent>();
        InputIntendComponent* intend = entity->GetComponent<InputIntendComponent>();

        if (!mapping || !intend) continue;

        intend->Reset();

        for (const auto& [key, pair] : mapping->directionMapping)
        {
            KeyState& ks = input.GetKey(key);

            if (ks.held)
            {
                switch (pair.first)
                {
                case InputAction::HORIZONTAL:
                    intend->x = pair.second;
                    break;
                case InputAction::VERTICAL:
                    intend->y = pair.second;
                    break;
                
                default:
                    break;
                }
            }
        }

        for (const auto& [key, action] : mapping->actionMapping)
        {
            KeyState& ks = input.GetKey(key);

            if (ks.pressed)
            {
                intend->actions[action] = true;
            }
        }
    }
}