#include "Entities/Components/AI.h"

AIComponent::~AIComponent() = default;

void AIComponent::Reset()
{
    memory.clear();
}
