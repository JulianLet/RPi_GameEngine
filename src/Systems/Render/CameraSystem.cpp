#include "CameraSystem.h"

#include "Managers/Game/World.h"

void CameraSystem::Update(World& world, float deltaTime)
{
    for (uint8_t e = 0; e < MAX_ENTITIES; e++)
    {
        if (!world.entities[e].mask & CameraBit) continue;

        auto& camera = world.cameras[e];

        if (camera.currentZoom < camera.targetZoom - camera.zoomSpeed * deltaTime)
        {
            camera.currentZoom += camera.zoomSpeed * deltaTime;
        }
        else if (camera.currentZoom > camera.targetZoom + camera.zoomSpeed * deltaTime)
        {
            camera.currentZoom -= camera.zoomSpeed * deltaTime;
        }
        else
        {
            camera.currentZoom = camera.targetZoom;
        }
    }
}