#pragma once

#include <vector>
#include <memory>

class Renderer;
class Entity;
class CameraComponent;

class RenderSystem
{


    public:
    RenderSystem() = default;
    ~RenderSystem() = default;

    void Render(const std::vector<std::unique_ptr<Entity>>& entities, Renderer& renderer);
    void Render(const std::vector<std::unique_ptr<Entity>>& entities, Renderer& renderer, CameraComponent* camera);
};