#pragma once

#include <vector>
#include <memory>

class Entity;
class Renderer;

class UIRenderSystem
{

    public: 
    UIRenderSystem() = default;
    ~UIRenderSystem() = default;

    void Render(const std::vector<std::unique_ptr<Entity>>& entities, Renderer& renderer);
};