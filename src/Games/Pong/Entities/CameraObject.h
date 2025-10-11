#pragma once

#include "Entities/Entity.h"

class CameraObject : public Entity
{


    public:
    CameraObject(Vector2 position, float startZoom);
    ~CameraObject() = default;
};