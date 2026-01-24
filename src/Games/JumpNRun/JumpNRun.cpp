#include "JumpNRun.h"

#include "Systems/Events/EventSystem.h"

#include "Systems/Events/Event.h"

#include "Entities/Common/Prototyping/SideScrollerPlayer.h"
#include "Entities/Common/Prototyping/StaticWall.h"
#include "Entities/Common/Prototyping/FollowCamera.h"
#include "Entities/Common/UI/UITextObject.h"

#include "Entities/Components/Core/TransformComponent.h"
#include "Entities/Components/UI/UITextComponent.h"
#include "Entities/Components/Render/CameraComponent.h"
#include "Entities/Components/Render/RenderableComponent.h"

#include "Games/JumpNRun/Entities/JRGoal.h"
#include "Games/JumpNRun/Entities/Cloud.h"
#include "Games/JumpNRun/Entities/UITimerText.h"

#include <algorithm>

JumpNRun::JumpNRun(GameManager &manager) : Game("Jump 'N' Run", manager), actionSystem(this, &jrManager), entityManager(myEntities, this), eventComponentSystem(myEntities, this), eventComponentSystemUI(myUIElements, this)
{
    // --- World entities ---
    auto& es = EventSystem::GetInstance();
    
    es.DispatchEvent(EventSpawnEntity(new StaticWall(Vector2(-50, 180), Vector2(210, 50), Color::BLACK, "Ground"))); //ground
    es.DispatchEvent(EventSpawnEntity(new StaticWall(Vector2( 80, 150), Vector2(120, 30), Color::BLACK, "Ground"))); //first step
    es.DispatchEvent(EventSpawnEntity(new StaticWall(Vector2(100, 110), Vector2(100, 40), Color::BLACK, "Ground"))); //second step
    es.DispatchEvent(EventSpawnEntity(new StaticWall(Vector2(190,  30), Vector2( 10, 80), Color::BLACK, "Ground"))); //wall right
    es.DispatchEvent(EventSpawnEntity(new StaticWall(Vector2(110,  30), Vector2( 50, 10), Color::BLACK, "Ground"))); //air right
    es.DispatchEvent(EventSpawnEntity(new StaticWall(Vector2( 32,  45), Vector2( 30, 10), Color::BLACK, "Ground"))); //air left
    es.DispatchEvent(EventSpawnEntity(new StaticWall(Vector2(-20, -30), Vector2( 10, 85), Color::BLACK, "Ground"))); //wall left
    es.DispatchEvent(EventSpawnEntity(new StaticWall(Vector2( 20, -40), Vector2( 70, 10), Color::BLACK, "Ground"))); //top

    auto* player =  new SideScrollerPlayer(Vector2(0, 110), Vector2(8, 10), 45, Color::BLUE, 85);
    playerTransform = player->GetComponent<TransformComponent>();
    es.DispatchEvent(EventSpawnEntity(player));

    es.DispatchEvent(EventSpawnEntity(new JRGoal(Vector2( 75, -55), Vector2(10, 10)))); //goal

    es.DispatchEvent(EventSpawnEntity(&jrManager));

    auto* cam = new FollowCamera(playerTransform, 1, 1, 30);
    camRef = cam->GetComponent<CameraComponent>();
    es.DispatchEvent(EventSpawnEntity(cam));

    for (int i = 0; i < 10; i++)
    {
        es.DispatchEvent(EventSpawnEntity(new Cloud(i, 10)));
    }

    // --- UI entities ---
    myUIElements.push_back(std::make_unique<UITextObject>(Vector2(26, 8), "Jump 'N' Run", Color::BLACK, -1));
    myUIElements.push_back(std::make_unique<UITimerText>(Vector2(5, 150), Color::WHITE, -1));
}

void JumpNRun::Update(Input &input, float deltaTime)
{
    inputSystem.Update(myEntities, input);
    actionSystem.Update(myEntities, deltaTime);
    entityManager.Update();
    uiUpdateSystem.Update(myUIElements, input, myGameManager);

    if (runGame)
    {
        timerSystem.Update(myEntities, deltaTime);
        timerSystem.Update(myUIElements, deltaTime);
        aiSystem.Update(myEntities, deltaTime);
        movementSystem.Update(myEntities, deltaTime);
        physicsSystem.Update(myEntities, deltaTime);
        collisionSystem.Update(myEntities);
        physicsSystem.ResolveCollisions(myEntities, deltaTime);
        cameraSystem.Update(myEntities, deltaTime);
    }

    if (playerTransform->currentPosition.y > 250)
    {
        ResetGame();
    }
}

void JumpNRun::Render(Renderer &renderer)
{
    renderer.Clear(backgroundColor);

    renderSystem.Render(myEntities, renderer, camRef);
    uiRenderSystem.Render(myUIElements, renderer); //ui on top of world
}

void JumpNRun::StartGame()
{
    runGame = true;
}

void JumpNRun::ResetGame()
{
    runGame = false;

    for (auto& e : myEntities)
    {
        e->Reset();
    }

    for (auto& e : myUIElements)
    {
        e->Reset();
    }
}