#pragma once

#include "Entities/Component.h"

#include "Common.hpp"

class TransformComponent;

struct CameraComponent : public Component
{
    float initialZoom;
    float currentZoom;
    float targetZoom;
    float zoomSpeed = 2;

    public:
    CameraComponent(float initialZoom, float targetZoom);
    ~CameraComponent() override;

    void Reset() override;
};