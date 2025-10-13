#include "StaticCamera.h"

#include "Entities/Components/Render/CameraComponent.h"
#include "Entities/Components/Core/TransformComponent.h"

StaticCamera::StaticCamera(Vector2 position, float initZoom, float targetZoom)
{
    AddComponent<CameraComponent>(initZoom, targetZoom);
    AddComponent<TransformComponent>(position, Vector2(0,0));
}