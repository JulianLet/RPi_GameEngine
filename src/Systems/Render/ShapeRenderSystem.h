#pragma once

struct World;
class Renderer;

class ShapeRenderSystem
{
    public:
    ShapeRenderSystem() = default;
    ~ShapeRenderSystem() = default;

    void Render(World& world, Renderer& renderer, int layer);
};