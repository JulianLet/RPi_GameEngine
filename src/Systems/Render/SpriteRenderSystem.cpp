#include "SpriteRenderSystem.h"

#include "Hardware/Renderer.h"
#include "Hardware/DisplayST7735.h"

#include "Managers/Game/World.h"

void SpriteRenderSystem::Render(World& world, Renderer &renderer)
{
    Vector2 cameraPos = Vector2(0,0);
    float currentZoom = 1;

    //adjust to current camera position and zoom
    if (world.activeCamera != INVALID_ENTITY)
    {
        auto& camTransform = world.transforms[world.activeCamera];
        cameraPos = camTransform.currentPosition;

        currentZoom = world.cameras[world.activeCamera].currentZoom;
        cameraPos.x -= ST7735::WIDTH / (2 * currentZoom);
        cameraPos.y -= ST7735::HEIGHT / (2 * currentZoom); //cameraPos in middle of screen
    }

    uint32_t requiredMask = RenderableBit | TransformBit | SpriteBit; 
    for (uint8_t e = 0; e < MAX_ENTITIES; e++)
    {
        if (!world.entities[e].isAlive) continue;
        if ((world.entities[e].mask & requiredMask) != requiredMask) continue;

        auto& renderable = world.renderables[e];
        auto& transform = world.transforms[e];
        auto& sprite = world.sprites[e];
        
        // Calculate position relative to camera with parallax
        float parallaxX = cameraPos.x * renderable.parallaxFactor;
        float parallaxY = cameraPos.y * renderable.parallaxFactor;
        
        float screenX = (transform.currentPosition.x - parallaxX) * currentZoom;
        float screenY = (transform.currentPosition.y - parallaxY) * currentZoom;

        float prevX = (transform.lastPosition.x - parallaxX) * currentZoom;
        float prevY = (transform.lastPosition.y - parallaxY) * currentZoom;

        float screenWidth = transform.currentSize.x * currentZoom;
        float screenHeight = transform.currentSize.y * currentZoom;

        // cull entities outside screen
        if (screenX + screenWidth < 0 || screenX > ST7735::WIDTH ||
            screenY + screenHeight < 0 || screenY > ST7735::HEIGHT)
        {
            continue;
        }

        bool flipX = false;

        renderer.DrawSprite((int)screenX, (int)screenY, world.spriteCache.sprites[sprite.frame], currentZoom, flipX);

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