#include "Collider.h"

ColliderComponent::~ColliderComponent() = default;

void ColliderComponent::Reset()
{
    currentCollisionsPos.clear();
}
