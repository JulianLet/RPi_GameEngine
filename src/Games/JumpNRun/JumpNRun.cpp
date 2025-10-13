#include "JumpNRun.h"

#include "Systems/Events/EventSystem.h"

#include "Systems/Events/Event.h"

#include "Entities/Common/Prototyping/SideScrollerPlayer.h"
#include "Entities/Common/Prototyping/StaticWall.h"
#include "Entities/Common/Prototyping/FollowCamera.h"
#include "Entities/Common/UI/UITextObject.h"

#include "Entities/Components/Core/TransformComponent.h"
#include "Entities/Components/Render/CameraComponent.h"
#include "Entities/Components/Render/RenderableComponent.h"

#include "Games/JumpNRun/Entities/Cloud.h"

#include <algorithm>

JumpNRun::JumpNRun(GameManager &manager) : Game("Jump 'N' Run", manager), entityManager(myEntities, this), eventComponentSystem(myEntities, this), eventComponentSystemUI(myUIElements, this)
{
    auto& es = EventSystem::GetInstance();

    // --- World entities ---
    auto* player =  new SideScrollerPlayer(Vector2(65,50), Vector2(5,8), 40, Color::BLUE, 3);
    TransformComponent* playerTransform = player->GetComponent<TransformComponent>();
    es.DispatchEvent(EventSpawnEntity(player));

    es.DispatchEvent(EventSpawnEntity(new StaticWall(Vector2(-50,130), Vector2(500,40), Color::BLACK, "Ground")));
    es.DispatchEvent(EventSpawnEntity(new StaticWall(Vector2(80,120),  Vector2(200,40), Color::BLACK, "Ground")));
    es.DispatchEvent(EventSpawnEntity(new StaticWall(Vector2(100,105), Vector2(180,40), Color::BLACK, "Ground")));
    es.DispatchEvent(EventSpawnEntity(new StaticWall(Vector2(120,85),  Vector2(160,40), Color::BLACK, "Ground")));
    es.DispatchEvent(EventSpawnEntity(new StaticWall(Vector2(140,60),  Vector2(140,40), Color::BLACK, "Ground")));

    auto* cam = new FollowCamera(playerTransform, 1, 3, 30);
    camObj = cam->GetComponent<CameraComponent>();
    es.DispatchEvent(EventSpawnEntity(cam));

    for (int i = 0; i < 10; i++)
    {
        es.DispatchEvent(EventSpawnEntity(new Cloud(i, 10)));
    }

    // --- UI entities ---
    myUIElements.push_back(std::make_unique<UITextObject>(Vector2(26, 8), "Jump 'N' Run", Color::BLACK, -1));

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