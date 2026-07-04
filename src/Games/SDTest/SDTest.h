#pragma once

#include "Common.hpp"
#include "Games/Game.h"

#include "Entities/Common/CommonFactory.h"
#include "Entities/Common/CommonUIFactory.h"
#include "Games/SDTest/Systems/SDFactory.h"
#include "Games/SDTest/Systems//PlayerAnimationSystem.h"

#include "Systems/Systems.h"
#include "Managers/Managers.h"

class SDTest : public Game
{
    public:
    bool runGame = false;

    private:
    // Variables
    uint16_t backgroundColor = Color::DARKGRAY;

    // Factories
    World world;
    CommonFactory myCommonFactory;
    CommonUIFactory myCommonUIFactory;
    SDFactory mySDFactory;
    
    // General
    AISystem myAISystem;
    TimerSystem myTimerSystem;
    EventComponentSystem myEventSystem;
    InputSystem myInputSystem;
    
    // Movement
    FollowMovementSystem myFollowSystem;
    InputMovementSystem myInputMoveSystem;
    JumpSystem myJumpSystem;
    MovementSystem myMovementSystem;
    
    // Physics
    CollisionSystem myCollisionSystem;
    PhysicsSystem myPhysicsSystem;
    
    // Rendering
    PlayerAnimationSystem myPlayerAnimSystem;
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
    SDTest(GameManager& manager);
    ~SDTest() = default;

    void Update(Input& input, float deltaTime) override;
    void Render(Renderer& renderer) override;
};