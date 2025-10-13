#pragma once

#include "Entities/Entity.h"

class StaticCamera : public Entity
{


    public:
    StaticCamera(Vector2 position, float initZoom, float targetZoom);
    ~StaticCamera() = default;
};