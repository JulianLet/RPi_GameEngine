#include "ColliderComponent.h"

ColliderComponent::~ColliderComponent() = default;

void ColliderComponent::Reset()
{
    currentCollisions.clear();
}
