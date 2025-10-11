#pragma once

#include "Entities/Entity.h"

class TransformComponent;

class JRCamera : public Entity
{


    public:
    JRCamera(Vector2 position, TransformComponent* target);
    ~JRCamera() = default;
};