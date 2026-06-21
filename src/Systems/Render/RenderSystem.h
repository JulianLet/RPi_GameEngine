#pragma once

struct World;
class Renderer;
class CameraComponent;

class RenderSystem
{
    public:
    RenderSystem() = default;
    ~RenderSystem() = default;

    void Render(World& world, Renderer& renderer);
    void Render(World& world, Renderer& renderer, CameraComponent* camera);
};