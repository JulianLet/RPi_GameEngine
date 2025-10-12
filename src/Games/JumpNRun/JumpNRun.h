#pragma once

#include "Common.h"
#include "Games/Game.h"

#include "Games/JumpNRun/Systems/JRActionSystem.h"

#include "Systems/InputSystem.h"
#include "Systems/EntityManager.h"
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

class JumpNRun : public Game
{
    public:
    bool runGame = false;

    private:
    uint16_t backgroundColor = Color::CYAN;
    
    InputSystem inputSystem;
    EntityManager entityManager;
    JRActionSystem actionSystem;
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

    CameraComponent* camObj;

    std::vector<std::unique_ptr<Entity>> myEntities;
    std::vector<std::unique_ptr<Entity>> myUIElements;

    public:
    JumpNRun(GameManager& manager);
    ~JumpNRun() = default;

    void Update(Input& input, float deltaTime) override;
    void Render(Renderer& renderer) override;
    
    private:
    void ResetGame();
};