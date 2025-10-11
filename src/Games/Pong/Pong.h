#pragma once

#include "Common.h"
#include "Games/Game.h"
#include "Systems/EntityManager.h"
#include "Systems/InputSystem.h"
#include "Games/Pong/Systems/PongActionSystem.h"
#include "Systems/CameraSystem.h"
#include "Systems/AISystem.h"
#include "Systems/MovementSystem.h"
#include "Systems/PhysicsSystem.h"
#include "Systems/CollisionSystem.h"
#include "Systems/RenderSystem.h"
#include "Systems/UIRenderSystem.h"
#include "Systems/EventComponentSystem.h"
#include "Entities/Entity.h"

#include <vector>
#include <memory>

class Pong : public Game
{
    public:
    bool runGame = false;

    private:
    TransformComponent* ballTransform = nullptr;
    CameraComponent* camRef = nullptr;

    Vector2 playingFieldSize = {130, 100};
    uint16_t playingFieldColor = Color::GREEN;
    
    EntityManager entityManager;
    InputSystem inputSystem;
    PongActionSystem actionSystem;
    AISystem aiSystem;
    MovementSystem movementSystem;
    PhysicsSystem physicsSystem;
    CollisionSystem collisionSystem;
    EventComponentSystem eventComponentSystem;
    EventComponentSystem eventComponentSystemUI;
    CameraSystem cameraSystem;
    RenderSystem renderSystem;
    UIRenderSystem uiRenderSystem;


    std::vector<std::unique_ptr<Entity>> myEntities;
    std::vector<std::unique_ptr<Entity>> myUIElements;

    public:
    Pong(GameManager& manager);
    ~Pong() = default;

    void Update(Input& input, float deltaTime) override;
    void Render(Renderer& renderer) override;

    Entity* GetBall();
    
    private:
    void ResetGame();
};