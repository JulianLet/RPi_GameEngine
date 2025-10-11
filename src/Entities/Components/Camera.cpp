#include "Entities/Components/Camera.h"
#include "Camera.h"


CameraComponent::CameraComponent(float initialZoom, float targetZoom)
    : initialZoom(initialZoom), targetZoom(targetZoom)
{
    currentZoom = initialZoom;
}

CameraComponent::~CameraComponent() = default;

void CameraComponent::Reset()
{
    currentZoom = initialZoom;
}
