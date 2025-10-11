#include "JRCamera.h"

#include "Entities/Components/Camera.h"
#include "Entities/Components/Transform.h"
#include "Entities/Components/Movement.h"
#include "Entities/Components/FollowTarget.h"

JRCamera::JRCamera(Vector2 position, TransformComponent* target)
{
    AddComponent<CameraComponent>(1.f, 2.f);
    AddComponent<TransformComponent>(position, Vector2(0,0));
    AddComponent<MovementComponent>(30.f);
    AddComponent<FollowTargetComponent>(target);
};