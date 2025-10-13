#pragma once

#include "Entities/Entity.h"

class TransformComponent;

class FollowCamera : public Entity
{


    public:
    FollowCamera(TransformComponent* target, float initZoom, float targetZoom, float speed);
    ~FollowCamera() = default;
};