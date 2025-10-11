#pragma once

#include "Entities/Component.h"

struct RenderableComponent : public Component
{
    bool doRender;
    int layer;
    float parallaxFactor;

    RenderableComponent(int layer, bool doRender = true, float parallaxFactor = 1) : layer(layer), doRender(doRender), parallaxFactor(parallaxFactor) {};
    ~RenderableComponent() override;

    void Reset() override;
};