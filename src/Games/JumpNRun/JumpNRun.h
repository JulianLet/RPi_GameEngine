#pragma once

#include "Common.hpp"
#include "Games/Game.h"

#include "Entities/Common/CommonFactory.h"
#include "Entities/Common/CommonUIFactory.h"
#include "Games/JumpNRun/Systems/JRFactory.h"
#include "Games/JumpNRun/Systems/JRActionSystem.h"

#include "Systems/Systems.h"
#include "Managers/Managers.h"

class JumpNRun : public Game
{
    public:
    bool runGame = false;

    private:
    // Variables
    uint16_t playingFieldColor = Color::GREEN;

    // Factories
    World world;
    CommonFactory myCommonFactory;
    CommonUIFactory myCommonUIFactory;
    JRFactory myJRFactory;
    JRActionSystem myActionSystem;
    
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
    JumpNRun(GameManager& manager);
    ~JumpNRun() = default;

    void Update(Input& input, float deltaTime) override;
    void Render(Renderer& renderer) override;
    void ResetGame();
};