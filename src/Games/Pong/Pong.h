#pragma once

#include "Common.h"
#include "Games/Game.h"
#include "Entities/Entity.h"

#include "Games/Pong/Systems/PongActionSystem.h"

#include "Systems/AI/AISystem.h"
#include "Systems/Core/EntityManager.h"
#include "Systems/Events/EventComponentSystem.h"
#include "Systems/Input/InputSystem.h"
#include "Systems/Physics/MovementSystem.h"
#include "Systems/Physics/PhysicsSystem.h"
#include "Systems/Physics/CollisionSystem.h"
#include "Systems/Render/CameraSystem.h"
#include "Systems/Render/RenderSystem.h"
#include "Systems/UI/UIRenderSystem.h"
#include "Systems/UI/UIUpdateSystem.h"

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
    
    
    AISystem aiSystem;
    EntityManager entityManager;

    //Input
    InputSystem inputSystem;
    PongActionSystem actionSystem;

    //Physics
    MovementSystem movementSystem;
    PhysicsSystem physicsSystem;
    CollisionSystem collisionSystem;

    //Events
    EventComponentSystem eventComponentSystem;
    EventComponentSystem eventComponentSystemUI;

    //Render
    CameraSystem cameraSystem;
    RenderSystem renderSystem;

    //UI
    UIUpdateSystem uiUpdateSystem;
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