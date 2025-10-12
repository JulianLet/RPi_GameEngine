#pragma once

#include "Common.h"
#include "Games/Game.h"
#include "Systems/EntityManager.h"
#include "Systems/InputSystem.h"
#include "Games/Menu/Systems/MenuActionSystem.h"
#include "Systems/CameraSystem.h"
#include "Systems/AISystem.h"
#include "Systems/MovementSystem.h"
#include "Systems/PhysicsSystem.h"
#include "Systems/CollisionSystem.h"
#include "Systems/RenderSystem.h"
#include "Systems/UIRenderSystem.h"
#include "Systems/UIUpdateSystem.h"
#include "Systems/EventComponentSystem.h"
#include "Entities/Entity.h"

#include <vector>
#include <memory>

class Menu : public Game
{
    public:
    bool runGame = false;

    private:
    CameraComponent* camRef = nullptr;

    uint16_t backgroundColor = Color::BLUE;
    
    EntityManager entityManager;
    InputSystem inputSystem;
    MenuActionSystem actionSystem;
    AISystem aiSystem;
    MovementSystem movementSystem;
    PhysicsSystem physicsSystem;
    CollisionSystem collisionSystem;
    EventComponentSystem eventComponentSystem;
    EventComponentSystem eventComponentSystemUI;
    UIUpdateSystem uiUpdateSystem;
    CameraSystem cameraSystem;
    RenderSystem renderSystem;
    UIRenderSystem uiRenderSystem;


    std::vector<std::unique_ptr<Entity>> myEntities;
    std::vector<std::unique_ptr<Entity>> myUIElements;

    public:
    Menu(GameManager& manager);
    ~Menu() = default;

    void Update(Input& input, float deltaTime) override;
    void Render(Renderer& renderer) override;
};