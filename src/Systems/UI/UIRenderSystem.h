#pragma once

struct World;
class Renderer;

class UIRenderSystem
{
    public: 
    UIRenderSystem() = default;
    ~UIRenderSystem() = default;

    void Render(World& world, Renderer& renderer);
};