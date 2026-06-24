#pragma once

#include "Common.hpp"
#include "Games/Game.h"

#include "Games/Menu/Systems/MenuActionSystem.h"

#include "Systems/Systems.h"
#include "Managers/Managers.h"

#include <vector>
#include <memory>

struct CommonFactory;
struct MenuFactory;

class Menu : public Game
{
    public:
    bool runGame = false;

    private:
    World world;
    uint16_t backgroundColor = Color::BLUE;

    CommonFactory myCommonFactory;
    MenuFactory myFactory;
    
    AISystem myAISystem;
    TimerSystem myTimerSystem;
    EventComponentSystem myEventSystem;
    InputSystem myInputSystem;
    FollowMovementSystem myFollowSystem;
    InputMovementSystem myInputMoveSystem;
    MovementSystem myMovementSystem;
    CollisionSystem myCollisionSystem;
    PhysicsSystem myPhysicsSystem;

    AnimationSystem myAnimationSystem;
    CameraSystem myCameraSystem;
    ShapeRenderSystem myShapeRenderSystem;
    SpriteRenderSystem mySpriteRenderSystem;
    TilemapSystem myTilemapSystem;
    UIButtonSystem myUIButtonSystem;
    UITimerTextSystem myUITimerSystem;
    UIRenderSystem myUIRenderSystem;

    public:
    Menu(GameManager& manager);
    ~Menu() = default;

    void Update(Input& input, float deltaTime) override;
    void Render(Renderer& renderer) override;
};