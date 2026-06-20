#pragma once
#include "Common.hpp"


struct CameraComponent
{
    float currentZoom;
    float targetZoom;
    float zoomSpeed = 2;
};