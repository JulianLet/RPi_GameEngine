#include "Menu.h"

#include "Entities/Entity.h"
#include "Systems/Core/GameManager.h"

#include "Games/Pong/Pong.h"
#include "Games/JumpNRun/JumpNRun.h"

#include "Systems/Events/EventSystem.h"
#include "Systems/Events/Event.h"

#include "Entities/Components/Core/TransformComponent.h"
#include "Entities/Components/Render/RectangleComponent.h"
#include "Entities/Components/UI/UITextComponent.h"
#include "Entities/Components/UI/UIButtonComponent.h"
#include "Entities/Components/Render/RenderableComponent.h"

#include "Entities/Components/Render/CameraComponent.h"
#include "pico/stdlib.h"
#include <algorithm>

Menu::Menu(GameManager &manager) : Game("MENU", manager), entityManager(myEntities, this), eventComponentSystem(myEntities, this), eventComponentSystemUI(myUIElements, this)
{
    auto& es = EventSystem::GetInstance();

    // --- World entities ---


    // --- UI elements ---
    auto pongButton = std::make_unique<Entity>();
    pongButton->AddComponent<TransformComponent>(Vector2(10,30), Vector2(110,20));
    pongButton->AddComponent<RectangleComponent>(Color::WHITE, true);
    pongButton->AddComponent<UIButtonComponent>(Color::BLACK, Color::LIGHTGRAY);
    pongButton->GetComponent<UIButtonComponent>()->OnClicked = [](GameManager& manager)
    {
        //EventSystem::GetInstance().listenerEntries.clear();
        manager.SwitchGame(std::make_unique<Pong>(manager));
    };
    pongButton->AddComponent<RenderableComponent>(0);

    myUIElements.emplace_back(std::move(pongButton));
    
    auto pongButtonText = std::make_unique<Entity>();
    pongButtonText->AddComponent<TransformComponent>(Vector2(65-12, 37), Vector2(0,0));
    pongButtonText->AddComponent<UITextComponent>("PONG", Color::WHITE);
    pongButtonText->AddComponent<RenderableComponent>(0);
    myUIElements.emplace_back(std::move(pongButtonText));

    auto jnrButton = std::make_unique<Entity>();
    jnrButton->AddComponent<TransformComponent>(Vector2(10,60), Vector2(110,20));
    jnrButton->AddComponent<RectangleComponent>(Color::WHITE, true);
    jnrButton->AddComponent<UIButtonComponent>(Color::BLACK, Color::LIGHTGRAY);
    jnrButton->GetComponent<UIButtonComponent>()->OnClicked = [](GameManager& manager)
    {
        //EventSystem::GetInstance().listenerEntries.clear();
        manager.SwitchGame(std::make_unique<JumpNRun>(manager));
    };
    jnrButton->AddComponent<RenderableComponent>(0);

    myUIElements.emplace_back(std::move(jnrButton));
    
    auto jnrButtonText = std::make_unique<Entity>();
    jnrButtonText->AddComponent<TransformComponent>(Vector2(65-36, 67), Vector2(0,0));
    jnrButtonText->AddComponent<UITextComponent>("JUMP 'N' RUN", Color::WHITE);
    jnrButtonText->AddComponent<RenderableComponent>(0);
    myUIElements.emplace_back(std::move(jnrButtonText));
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
        physicsSystem.ResolveCollisions(myEntities);
        cameraSystem.Update(myEntities, deltaTime);
    }
}

void Menu::Render(Renderer &renderer)
{
    renderer.Clear(backgroundColor);

    renderSystem.Render(myEntities, renderer, camRef);
    uiRenderSystem.Render(myUIElements, renderer); //ui on top of world
}