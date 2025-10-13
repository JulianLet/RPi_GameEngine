#include "FollowCamera.h"

#include "Entities/Components/Core/TransformComponent.h"
#include "Entities/Components/Render/CameraComponent.h"
#include "Entities/Components/Core/MovementComponent.h"
#include "Entities/Components/Render/FollowTargetComponent.h"

FollowCamera::FollowCamera(TransformComponent* target, float initZoom, float targetZoom, float speed)
{
    AddComponent<CameraComponent>(initZoom, targetZoom);
    AddComponent<TransformComponent>(target->currentPosition, Vector2(0,0));
    AddComponent<MovementComponent>(speed);
    AddComponent<FollowTargetComponent>(target);
};