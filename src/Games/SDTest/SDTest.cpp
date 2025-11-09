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
#include "Entities/Components/Render/AnimationComponent.h"

#include "Entities/Components/Render/CameraComponent.h"
#include "pico/stdlib.h"
#include <algorithm>
#include <cstring>

SDTest::SDTest(GameManager &manager) : Game("SDTest", manager), entityManager(myEntities, this), eventComponentSystem(myEntities, this), eventComponentSystemUI(myUIElements, this)
{
    auto& es = EventSystem::GetInstance();

    // --- World entities ---
    auto* camera = new StaticCamera(Vector2(65, 81), 3.f, 0.5f);
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
    bR->AddComponent<AnimationComponent>(20,10);
    bR->GetComponent<AnimationComponent>()->AddAnimation(0, AnimationMode::BOUNCE, 3, 3, "recsheet.txt");
    es.DispatchEvent(EventSpawnEntity(bR));

    auto* lR = new Entity(); 
    lR->AddComponent<TransformComponent>(Vector2(50,30), Vector2(20,10));
    lR->AddComponent<RenderableComponent>(0);
    lR->AddComponent<AnimationComponent>(20,10);
    lR->GetComponent<AnimationComponent>()->AddAnimation(0, AnimationMode::LOOP, 3, 2, "recsheet.txt");
    es.DispatchEvent(EventSpawnEntity(lR));

    auto* sR = new Entity(); 
    sR->AddComponent<TransformComponent>(Vector2(90,30), Vector2(20,10));
    sR->AddComponent<RenderableComponent>(0);
    sR->AddComponent<AnimationComponent>(20,10);
    sR->GetComponent<AnimationComponent>()->AddAnimation(0, AnimationMode::SINGLE, 3, 1, "recsheet.txt");
    es.DispatchEvent(EventSpawnEntity(sR));


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