#include "JRCamera.h"

#include "Entities/Components/Core/TransformComponent.h"
#include "Entities/Components/Render/CameraComponent.h"
#include "Entities/Components/Core/MovementComponent.h"
#include "Entities/Components/Render/FollowTargetComponent.h"

JRCamera::JRCamera(Vector2 position, TransformComponent* target)
{
    AddComponent<CameraComponent>(1.f, 2.f);
    AddComponent<TransformComponent>(position, Vector2(0,0));
    AddComponent<MovementComponent>(30.f);
    AddComponent<FollowTargetComponent>(target);
};