#include "Games/Pong/Entities/CameraObject.h"

#include "Entities/Components/Render/CameraComponent.h"
#include "Entities/Components/Core/TransformComponent.h"
#include "Entities/Components/Core/MovementComponent.h"

CameraObject::CameraObject(Vector2 position, float startZoom)
{
    AddComponent<CameraComponent>(startZoom, 1.f);
    AddComponent<TransformComponent>(position, Vector2(0,0));
    AddComponent<MovementComponent>(1.f);
}