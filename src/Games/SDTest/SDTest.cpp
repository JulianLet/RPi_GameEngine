#include "SDTest.h"

#include "Entities/Entity.h"
#include "Systems/Core/GameManager.h"

#include "Systems/Events/EventSystem.h"
#include "Systems/Events/Event.h"

#include "Systems/Debug/DebugManager.h"
#include "Systems/Resource/ResourceManager.h"

#include "Entities/Common/Prototyping/StaticCamera.h"

#include "Entities/Components/Core/TransformComponent.h"
#include "Entities/Components/Render/RenderableComponent.h"
#include "Entities/Components/Render/SpriteComponent.h"

#include "Entities/Components/Render/CameraComponent.h"
#include "pico/stdlib.h"
#include <algorithm>
#include <cstring>

SDTest::SDTest(GameManager &manager) : Game("SDTest", manager), entityManager(myEntities, this), eventComponentSystem(myEntities, this), eventComponentSystemUI(myUIElements, this)
{
    auto& es = EventSystem::GetInstance();

    // --- World entities ---
    auto* camera = new StaticCamera(Vector2(65, 81), 1.f, 1.f);
    camRef = camera->GetComponent<CameraComponent>();
    es.DispatchEvent(EventSpawnEntity(camera));

    auto* blueRec = new Entity();
    blueRec->AddComponent<TransformComponent>(Vector2(10,10), Vector2(20,10));
    blueRec->AddComponent<RenderableComponent>(0);
    blueRec->AddComponent<SpriteComponent>(20, 10, "bluerec.txt");
    es.DispatchEvent(EventSpawnEntity(blueRec));
    
    auto* bR = new Entity(); 
    bR->AddComponent<TransformComponent>(Vector2(10,30), Vector2(20,10));
    bR->AddComponent<RenderableComponent>(0);
    bR->AddComponent<SpriteComponent>(20, 10, "recsheet.txt", 0);
    es.DispatchEvent(EventSpawnEntity(bR));

    auto* rR = new Entity();
    rR->AddComponent<TransformComponent>(Vector2(50,30), Vector2(20,10));
    rR->AddComponent<RenderableComponent>(0);
    rR->AddComponent<SpriteComponent>(20, 10, "recsheet.txt", 1);
    es.DispatchEvent(EventSpawnEntity(rR));
    
    auto* yR = new Entity();
    yR->AddComponent<TransformComponent>(Vector2(90,30), Vector2(20,10));
    yR->AddComponent<RenderableComponent>(0);
    yR->AddComponent<SpriteComponent>(20, 10, "recsheet.txt", 2);
    es.DispatchEvent(EventSpawnEntity(yR));

    // --- UI elements ---
}

void SDTest::Update(Input &input, float deltaTime)
{
    inputSystem.Update(myEntities, input);
    actionSystem.Update(myEntities);
    entityManager.Update();
    uiUpdateSystem.Update(myUIElements, input, myGameManager);

    if (runGame)
    {
        aiSystem.Update(myEntities, deltaTime);
        movementSystem.Update(myEntities, deltaTime);
        physicsSystem.Update(myEntities, deltaTime);
        collisionSystem.Update(myEntities);
        physicsSystem.ResolveCollisions(myEntities, deltaTime);
        cameraSystem.Update(myEntities, deltaTime);
    }
}

void SDTest::Render(Renderer &renderer)
{
    renderer.Clear(backgroundColor);

    renderSystem.Render(myEntities, renderer, camRef);
    uiRenderSystem.Render(myUIElements, renderer); //ui on top of world
}