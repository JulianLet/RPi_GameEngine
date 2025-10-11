#include "Rectangle.h"

RectangleComponent::~RectangleComponent() = default;

void RectangleComponent::Reset()
{
    currentColor = startColor;
}
