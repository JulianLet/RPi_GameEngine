#include "SDTest.h"

#include "Entities/Entity.h"
#include "Systems/Core/GameManager.h"

#include "Systems/Events/EventSystem.h"
#include "Systems/Events/Event.h"

#include "Systems/Debug/DebugManager.h"
#include "Systems/Resource/ResourceManager.h"

#include "Systems/Debug/DebugManager.h"

#include "Entities/Common/Prototyping/StaticCamera.h"

#include "Entities/Components/Core/TransformComponent.h"
#include "Entities/Components/Render/RenderableComponent.h"
#include "Entities/Components/Render/SpriteComponent.h"
#include "Entities/Components/Render/AnimationComponent.h"

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

    auto* redRec = new Entity();
    redRec->AddComponent<TransformComponent>(Vector2(10,10), Vector2(32,32));
    redRec->AddComponent<RenderableComponent>(0);
    redRec->AddComponent<SpriteComponent>(32, 32, "PngTest.bin", 0);
    es.DispatchEvent(EventSpawnEntity(redRec));

    auto* greenRec = new Entity();
    greenRec->AddComponent<TransformComponent>(Vector2(50,10), Vector2(32,32));
    greenRec->AddComponent<RenderableComponent>(0);
    greenRec->AddComponent<SpriteComponent>(32, 32, "PngTest.bin", 1);
    es.DispatchEvent(EventSpawnEntity(greenRec));    

    auto* ablueRec = new Entity();
    ablueRec->AddComponent<TransformComponent>(Vector2(90,10), Vector2(32,32));
    ablueRec->AddComponent<RenderableComponent>(0);
    ablueRec->AddComponent<SpriteComponent>(32, 32, "PngTest.bin", 2);
    es.DispatchEvent(EventSpawnEntity(ablueRec));

    auto* blueRec = new Entity();
    blueRec->AddComponent<TransformComponent>(Vector2(90,10), Vector2(32,32));
    blueRec->AddComponent<RenderableComponent>(0);
    blueRec->AddComponent<SpriteComponent>(32, 32, "PngTest.bin", 3);
    es.DispatchEvent(EventSpawnEntity(blueRec));

    // auto* animTest = new Entity();
    // animTest->AddComponent<TransformComponent>(Vector2(32, 80), Vector2(64,64));
    // animTest->AddComponent<RenderableComponent>(1);
    // animTest->AddComponent<AnimationComponent>(64,64);
    // animTest->GetComponent<AnimationComponent>()->AddAnimation(0, AnimationMode::LOOP, 11, 64, "animTest.bin");
    // es.DispatchEvent(EventSpawnEntity(animTest)); 


    // --- UI elements ---
    runGame = true;
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
        animationSystem.Update(myEntities, deltaTime);
        cameraSystem.Update(myEntities, deltaTime);
    }
}

void SDTest::Render(Renderer &renderer)
{
    renderer.Clear(backgroundColor);

    renderSystem.Render(myEntities, renderer, camRef);
    uiRenderSystem.Render(myUIElements, renderer); //ui on top of world
}