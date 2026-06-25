#pragma once

#include "Common.hpp"
#include "Games/Game.h"

#include "Entities/Common/CommonFactory.h"
#include "Entities/Common/CommonUIFactory.h"
#include "Games/Pong/Systems/PongFactory.h"#
#include "Games/Pong/Systems/PongActionSystem.h"

#include "Systems/Systems.h"
#include "Managers/Managers.h"

class Pong : public Game
{
    public:
    bool runGame = false;
    Vector2 playingFieldSize = {130, 100};

    private:    
    // Variables
    uint16_t playingFieldColor = Color::GREEN;

    // Factories
    World world;
    CommonFactory myCommonFactory;
    CommonUIFactory myCommonUIFactory;
    PongFactory myPongFactory;
    PongActionSystem myActionSystem;
    
    // General
    AISystem myAISystem;
    TimerSystem myTimerSystem;
    EventComponentSystem myEventSystem;
    InputSystem myInputSystem;
    
    // Movement
    FollowMovementSystem myFollowSystem;
    InputMovementSystem myInputMoveSystem;
    MovementSystem myMovementSystem;
    
    // Physics
    CollisionSystem myCollisionSystem;
    PhysicsSystem myPhysicsSystem;
    
    // Rendering
    AnimationSystem myAnimationSystem;
    CameraSystem myCameraSystem;
    ShapeRenderSystem myShapeRenderSystem;
    SpriteRenderSystem mySpriteRenderSystem;
    TilemapSystem myTilemapSystem;

    // UI
    UIButtonSystem myUIButtonSystem;
    UITimerTextSystem myUITimerSystem;
    UIRenderSystem myUIRenderSystem;

    public:
    Pong(GameManager& manager);
    ~Pong() = default;

    void Update(Input& input, float deltaTime) override;
    void Render(Renderer& renderer) override;
    void ResetGame();
};