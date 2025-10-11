#include "Entities/Components/InputIntend.h"

InputIntendComponent::~InputIntendComponent() = default;

void InputIntendComponent::Reset()
{
    x = 0;
    y = 0;
    actions.clear();
}
