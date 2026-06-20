#pragma once
#include "Entities/Component.h"
#include "Common.hpp"

struct TransformComponent
{
    Vector2 currentPosition = {};
    Vector2 lastPosition = {};
    Vector2 currentSize = {};
};