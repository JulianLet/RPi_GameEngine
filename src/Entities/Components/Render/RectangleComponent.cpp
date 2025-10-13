#include "RectangleComponent.h"

RectangleComponent::~RectangleComponent() = default;

void RectangleComponent::Reset()
{
    currentColor = startColor;
}
