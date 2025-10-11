#include "Transform.h"
#include "Hardware/Input.h"

TransformComponent::~TransformComponent() = default;

Vector2 TransformComponent::GetCenterPos()
{
    return currentPosition + (currentSize/2);
}

void TransformComponent::Reset()
{
    currentPosition = startPosition;
    lastPosition = startPosition;
    currentSize = startSize;
}
