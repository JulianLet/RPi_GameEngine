#pragma once
#include "Entities/Component.h"
#include "Common.hpp"

struct TransformComponent : public Component
{
    Vector2 startPosition = {};
    Vector2 currentPosition = {};
    Vector2 lastPosition = {};

    Vector2 startSize = {};
    Vector2 currentSize = {};

    TransformComponent(Vector2 position, Vector2 size) 
        : startPosition(position), startSize(size)
    {
        currentPosition = startPosition;
        lastPosition = startPosition;
        currentSize = startSize;
    };

    ~TransformComponent() override;

    Vector2 GetCenterPos();

    void Reset() override;
};