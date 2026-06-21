#include "InputSystem.h"

#include "Hardware/Input.h"
#include "Managers/Game/World.h"

void InputSystem::Update(World& world, Input &input)
{
    uint32_t requiredMask = InputIntentBit | InputMappingBit;

    for (uint8_t e = 0; e < MAX_ENTITIES; e++)
    {
        if ((world.entities[e].mask & requiredMask) != requiredMask) continue;

        auto& mapping = world.inputMappings[e];
        auto& intend = world.inputIntends[e];

        // reset old inputs
        intend.x = 0;
        intend.y = 0;

        for (const auto& [key, action] : mapping.actionMapping)
        {
                intend.actions[action] = false;
        }      

        //update movement based on hardware input
        for (const auto& [key, pair] : mapping.directionMapping)
        {
            KeyState& ks = input.GetKey(key);

            if (ks.held)
            {
                switch (pair.first)
                {
                case InputAction::HORIZONTAL:
                    intend.x = pair.second;
                    break;
                case InputAction::VERTICAL:
                    intend.y = pair.second;
                    break;
                
                default:
                    break;
                }
            }
        }

        //update actions based on hardware input
        for (const auto& [key, action] : mapping.actionMapping)
        {
            KeyState& ks = input.GetKey(key);

            if (ks.pressed)
            {
                intend.actions[action] = true;
            }
        }
    }
}