#include "RenderSystem.h"

#include "Hardware/Renderer.h"
#include "Hardware/DisplayST7735.h"

#include "Entities/Entity.h"
#include "Entities/Components/Core/TransformComponent.h"
#include "Entities/Components/Render/RectangleComponent.h"
#include "Entities/Components/Render/RenderableComponent.h"
#include "Entities/Components/Render/CameraComponent.h"


void RenderSystem::Render(const std::vector<std::unique_ptr<Entity>>& entities, Renderer &renderer)
{
    for (auto& entity : entities)
    {
        TransformComponent* transform = entity->GetComponent<TransformComponent>();
        RectangleComponent* rectangle = entity->GetComponent<RectangleComponent>();

        if (!rectangle || !transform) continue;

        renderer.DrawRectangle(
            (int)transform->currentPosition.x,
            (int)transform->currentPosition.y,
            (int)transform->currentSize.x,
            (int)transform->currentSize.y,
            rectangle->currentColor,
            rectangle->filled
        );
    }
}

void RenderSystem::Render(const std::vector<std::unique_ptr<Entity>>& entities, Renderer &renderer, CameraComponent* camera)
{
    Vector2 cameraPos = Vector2(0,0);
    float currentZoom = 1;

    if (camera)
    {
        auto* camTransform = camera->owner->GetComponent<TransformComponent>();
        if (camTransform) cameraPos = camera->owner->GetComponent<TransformComponent>()->currentPosition;
        
        currentZoom = camera->currentZoom;
        cameraPos.x -= ST7735::WIDTH / (2 * currentZoom);
        cameraPos.y -= ST7735::HEIGHT / (2 * currentZoom); //cameraPos in middle of screen
    }

    for (auto& entity : entities)
    {
        RenderableComponent* renderable = entity->GetComponent<RenderableComponent>();
        TransformComponent* transform = entity->GetComponent<TransformComponent>();
        RectangleComponent* rectangle = entity->GetComponent<RectangleComponent>();

        if (!transform || !rectangle || !renderable || !renderable->doRender) continue;

        float prevX = (transform->lastPosition.x - cameraPos.x) * currentZoom;
        float prevY = (transform->lastPosition.y - cameraPos.y) * currentZoom;
        
        // Calculate position relative to camera with parallax
        float parallaxX = cameraPos.x * renderable->parallaxFactor;
        float parallaxY = cameraPos.y * renderable->parallaxFactor;

        float screenX = (transform->currentPosition.x - parallaxX) * currentZoom;
        float screenY = (transform->currentPosition.y - parallaxY) * currentZoom;

        float width = transform->currentSize.x * currentZoom;
        float height = transform->currentSize.y * currentZoom;

        // Optional: cull entities outside screen
        if (screenX + width < 0 || screenX > ST7735::WIDTH ||
            screenY + height < 0 || screenY > ST7735::HEIGHT)
        {
            continue;
        }
    
        renderer.DrawRectangle(
            (int)prevX,
            (int)prevY,
            (int)width,
            (int)height,
            Color::RED,
            rectangle->filled
        );

        renderer.DrawRectangle(
            (int)screenX,
            (int)screenY,
            (int)width,
            (int)height,
            rectangle->currentColor,
            rectangle->filled
        );
    }
}