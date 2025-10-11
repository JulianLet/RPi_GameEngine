#include "Movement.h"

MovementComponent::~MovementComponent() = default;

void MovementComponent::Reset()
{
    currentSpeed = startSpeed;
}
