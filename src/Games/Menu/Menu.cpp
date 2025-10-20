#include "Menu.h"

#include "Entities/Entity.h"
#include "Systems/Core/GameManager.h"

#include "Games/Pong/Pong.h"
#include "Games/JumpNRun/JumpNRun.h"
#include "Games/SDTest/SDTest.h"

#include "Systems/Events/EventSystem.h"
#include "Systems/Events/Event.h"

#include "Systems/Debug/DebugManager.h"

#include "Entities/Common/Prototyping/StaticCamera.h"
#include "Entities/Common/UI/UIButtonObject.h"
#include "Entities/Common/UI/UITextObject.h"

#include "Entities/Components/UI/UIButtonComponent.h"

#include "Entities/Components/Render/CameraComponent.h"
#include "pico/stdlib.h"
#include <algorithm>

Menu::Menu(GameManager &manager) : Game("MENU", manager), entityManager(myEntities, this), eventComponentSystem(myEntities, this), eventComponentSystemUI(myUIElements, this)
{
    auto& es = EventSystem::GetInstance();

    // --- World entities ---
    auto* camera = new StaticCamera(Vector2(65, 81), 1.f, 1.f);
    camRef = camera->GetComponent<CameraComponent>();
    es.DispatchEvent(EventSpawnEntity(camera));

    // --- UI elements ---
    auto pongButton = std::make_unique<UIButtonObject>(Vector2(10,30), Vector2(110,20), Color::BLACK, Color::YELLOW, 0);
    pongButton->GetComponent<UIButtonComponent>()->OnClicked = [](GameManager& manager)
    {
        manager.SwitchGame(std::make_unique<Pong>(manager));
    };
    myUIElements.emplace_back(std::move(pongButton));
    
    auto pongButtonText = std::make_unique<UITextObject>(Vector2(65-12, 37), "PONG", Color::WHITE, -1);
    myUIElements.emplace_back(std::move(pongButtonText));

    auto jnrButton = std::make_unique<UIButtonObject>(Vector2(10,60), Vector2(110,20), Color::BLACK, Color::YELLOW, 0);
    jnrButton->GetComponent<UIButtonComponent>()->OnClicked = [](GameManager& manager)
    {
        manager.SwitchGame(std::make_unique<JumpNRun>(manager));
    };

    myUIElements.emplace_back(std::move(jnrButton));
    
    auto jnrButtonText = std::make_unique<UITextObject>(Vector2(65-36, 67),"JUMP 'N' RUN", Color::WHITE, -1);
    myUIElements.emplace_back(std::move(jnrButtonText));

    auto sdButton = std::make_unique<UIButtonObject>(Vector2(10,90), Vector2(110,20), Color::BLACK, Color::YELLOW, 0);
    sdButton->GetComponent<UIButtonComponent>()->OnClicked = [](GameManager& manager)
    {
        manager.SwitchGame(std::make_unique<SDTest>(manager));
    };

    myUIElements.emplace_back(std::move(sdButton));
    
    auto sdButtonText = std::make_unique<UITextObject>(Vector2(65-21, 97),"SD CARD", Color::WHITE, -1);
    myUIElements.emplace_back(std::move(sdButtonText));
}

void Menu::Update(Input &input, float deltaTime)
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

void Menu::Render(Renderer &renderer)
{
    renderer.Clear(backgroundColor);

    renderSystem.Render(myEntities, renderer, camRef);
    uiRenderSystem.Render(myUIElements, renderer); //ui on top of world
}