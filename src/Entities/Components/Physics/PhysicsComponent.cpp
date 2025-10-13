#include "PhysicsComponent.h"

PhysicsComponent::~PhysicsComponent() = default;

void PhysicsComponent::Reset()
{
    currentVelocity = startVelocity;
}