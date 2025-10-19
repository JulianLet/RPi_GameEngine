#include "RenderSystem.h"

#include "Hardware/Renderer.h"
#include "Hardware/DisplayST7735.h"

#include "Entities/Entity.h"
#include "Entities/Components/Core/TransformComponent.h"
#include "Entities/Components/Render/RectangleComponent.h"
#include "Entities/Components/Render/RenderableComponent.h"
#include "Entities/Components/Render/CameraComponent.h"
#include "Entities/Components/Physics/ColliderComponent.h"


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

    //adjust to current camera position and zoom
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
        
        // Calculate position relative to camera with parallax
        float parallaxX = cameraPos.x * renderable->parallaxFactor;
        float parallaxY = cameraPos.y * renderable->parallaxFactor;
        
        float screenX = (transform->currentPosition.x - parallaxX) * currentZoom;
        float screenY = (transform->currentPosition.y - parallaxY) * currentZoom;

        float prevX = (transform->lastPosition.x - parallaxX) * currentZoom;
        float prevY = (transform->lastPosition.y - parallaxY) * currentZoom;

        float width = transform->currentSize.x * currentZoom;
        float height = transform->currentSize.y * currentZoom;

        // cull entities outside screen
        if (screenX + width < 0 || screenX > ST7735::WIDTH ||
            screenY + height < 0 || screenY > ST7735::HEIGHT)
        {
            continue;
        }
    
        //render position of last frame
        // renderer.DrawRectangle(
        //     (int)prevX,
        //     (int)prevY,
        //     (int)width,
        //     (int)height,
        //     Color::RED,
        //     rectangle->filled
        // );

        //render actual rec
        renderer.DrawRectangle(
            (int)screenX,
            (int)screenY,
            (int)width,
            (int)height,
            rectangle->currentColor,
            rectangle->filled
        );


        // //render collision normal
        // auto* collider = entity->GetComponent<ColliderComponent>();
        // if (collider)
        // {
        //     for (auto col : collider->currentCollisions)
        //     {
        //         for (int i = 0; i < 15; i++)
        //         {
        //             renderer.SetPixel(screenX + i * col.second.collisionNormal.x, screenY + i * col.second.collisionNormal.y, Color::MAGENTA);
        //         }
        //     }
        // }
    }
}