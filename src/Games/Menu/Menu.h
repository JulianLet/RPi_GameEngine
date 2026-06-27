#pragma once

#include "Common.hpp"
#include "Games/Game.h"

#include "Systems/Systems.h"
#include "Managers/Managers.h"
#include "Entities/Common/CommonFactory.h"
#include "Games/Menu/Systems/MenuFactory.h"

class Menu : public Game
{
    public:
    bool runGame = false;

    private:
    // Variables
    uint16_t backgroundColor = Color::BLUE;
    
    // Factories
    World world;
    CommonFactory myCommonFactory;
    MenuFactory myFactory;
    
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
    Menu(GameManager& manager);
    ~Menu() = default;

    void Update(Input& input, float deltaTime) override;
    void Render(Renderer& renderer) override;
};