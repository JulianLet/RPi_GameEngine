#include "RenderSystem.h"

#include "Hardware/Renderer.h"
#include "Hardware/DisplayST7735.h"

#include "Entities/Entity.h"
#include "Entities/Components/Core/TransformComponent.h"
#include "Entities/Components/Render/RectangleComponent.h"
#include "Entities/Components/Render/SpriteComponent.h"
#include "Entities/Components/Render/AnimationComponent.h"
#include "Entities/Components/Render/RenderableComponent.h"
#include "Entities/Components/Render/CameraComponent.h"
#include "Entities/Components/Tiles/TilemapComponent.h"
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
        auto* renderable = entity->GetComponent<RenderableComponent>();
        auto* transform = entity->GetComponent<TransformComponent>();
        auto* rectangle = entity->GetComponent<RectangleComponent>();
        auto* sprite = entity->GetComponent<SpriteComponent>();
        auto* animation = entity->GetComponent<AnimationComponent>();
        auto* tilemap = entity->GetComponent<TilemapComponent>();

        if (!transform || !renderable || !renderable->doRender) continue;
        
        // Calculate position relative to camera with parallax
        float parallaxX = cameraPos.x * renderable->parallaxFactor;
        float parallaxY = cameraPos.y * renderable->parallaxFactor;
        
        float screenX = (transform->currentPosition.x - parallaxX) * currentZoom;
        float screenY = (transform->currentPosition.y - parallaxY) * currentZoom;

        float prevX = (transform->lastPosition.x - parallaxX) * currentZoom;
        float prevY = (transform->lastPosition.y - parallaxY) * currentZoom;

        float screenWidth = transform->currentSize.x * currentZoom;
        float screenHeight = transform->currentSize.y * currentZoom;

        // cull entities outside screen
        if (screenX + screenWidth < 0 || screenX > ST7735::WIDTH ||
            screenY + screenHeight < 0 || screenY > ST7735::HEIGHT)
        {
            continue;
        }

        if (rectangle)
        {
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
                (int)screenWidth,
                (int)screenHeight,
                rectangle->currentColor,
                rectangle->filled
            );
        }

        if (sprite)
        {
            renderer.DrawSprite((int)screenX, (int)screenY, sprite->width, sprite->height, sprite->pixels, currentZoom);
        }

        if (animation)
        {
            renderer.DrawSprite((int)screenX, (int)screenY, animation->width, animation->height, animation->GetCurrentFrame(), currentZoom);
        }

        if (tilemap)
        {
            tilemap->Render(renderer, (int)screenX, (int)screenY , currentZoom);
        }

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