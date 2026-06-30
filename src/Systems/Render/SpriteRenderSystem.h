#pragma once

struct World;
class Renderer;

class SpriteRenderSystem
{
    public:
    SpriteRenderSystem() = default;
    ~SpriteRenderSystem() = default;

    void Render(World& world, Renderer& renderer, int layer);
};