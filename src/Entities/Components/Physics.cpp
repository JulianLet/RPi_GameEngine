#include "Entities/Components/Physics.h"

PhysicsComponent::~PhysicsComponent() = default;

void PhysicsComponent::Reset()
{
    currentVelocity = startVelocity;
}