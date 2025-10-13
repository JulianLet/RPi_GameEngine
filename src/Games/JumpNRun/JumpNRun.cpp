#include "JumpNRun.h"

#include "Systems/Events/EventSystem.h"

#include "Systems/Events/Event.h"

#include "Entities/Components/Core/TransformComponent.h"
#include "Entities/Components/Render/CameraComponent.h"
#include "Entities/Components/Render/RenderableComponent.h"

#include "Games/Pong/Entities/TextObject.h"
#include "Games/JumpNRun/Entities/Plattform.h"
#include "Games/JumpNRun/Entities/Player.h"
#include "Games/JumpNRun/Entities/Cloud.h"
#include "Games/JumpNRun/Entities/JRCamera.h"

#include <algorithm>

JumpNRun::JumpNRun(GameManager &manager) : Game("Jump 'N' Run", manager), entityManager(myEntities, this), eventComponentSystem(myEntities, this), eventComponentSystemUI(myUIElements, this)
{
    auto& es = EventSystem::GetInstance();

    // --- World entities ---

    auto* player =  new Player(Vector2(65,50));
    TransformComponent* playerTransform = player->GetComponent<TransformComponent>();
    es.DispatchEvent(EventSpawnEntity(player));

    es.DispatchEvent(EventSpawnEntity(new Plattform(Vector2(-50,130), Vector2(500,40))));
    es.DispatchEvent(EventSpawnEntity(new Plattform(Vector2(80,120),  Vector2(200,40))));
    es.DispatchEvent(EventSpawnEntity(new Plattform(Vector2(100,105), Vector2(180,40))));
    es.DispatchEvent(EventSpawnEntity(new Plattform(Vector2(120,85),  Vector2(160,40))));
    es.DispatchEvent(EventSpawnEntity(new Plattform(Vector2(140,60),  Vector2(140,40))));

    auto* cam = new JRCamera(Vector2(65,80), playerTransform);
    camObj = cam->GetComponent<CameraComponent>();
    es.DispatchEvent(EventSpawnEntity(cam));

    for (int i = 0; i < 10; i++)
    {
        es.DispatchEvent(EventSpawnEntity(new Cloud(i, 10)));
    }

    // --- UI entities ---
    myUIElements.push_back(std::make_unique<TextObject>(Vector2(26, 8), "Jump 'N' Run", Color::BLACK));

    runGame = true; //temp
}

void JumpNRun::Update(Input &input, float deltaTime)
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
        physicsSystem.ResolveCollisions(myEntities);
        cameraSystem.Update(myEntities, deltaTime);
    }
}

void JumpNRun::Render(Renderer &renderer)
{
    renderer.Clear(backgroundColor);

    renderSystem.Render(myEntities, renderer, camObj);
    uiRenderSystem.Render(myUIElements, renderer); //ui on top of world
}

void JumpNRun::ResetGame()
{
    runGame = false;

    for (auto& e : myEntities)
    {
        e->Reset();
    }
}