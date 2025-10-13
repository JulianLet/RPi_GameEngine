#include "CameraSystem.h"

#include "Entities/Entity.h"
#include "Entities/Components/Render/CameraComponent.h"

void CameraSystem::Update(const std::vector<std::unique_ptr<Entity>> &entities, float deltaTime)
{
    //currently just updates zoom
    for (auto& entity : entities)
    {
        CameraComponent* camera = entity->GetComponent<CameraComponent>();

        if (!camera) continue;

        if (camera->currentZoom < camera->targetZoom - camera->zoomSpeed * deltaTime)
        {
            camera->currentZoom += camera->zoomSpeed * deltaTime;
        }
        else if (camera->currentZoom > camera->targetZoom + camera->zoomSpeed * deltaTime)
        {
            camera->currentZoom -= camera->zoomSpeed * deltaTime;
        }
        else
        {
            camera->currentZoom = camera->targetZoom;
        }
    }
}