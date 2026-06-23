#pragma once

#include "Common.hpp"
#include "Games/Game.h"

#include "Games/Menu/Systems/MenuActionSystem.h"

#include "Systems/Systems.h"
#include "Managers/Managers.h"

#include <vector>
#include <memory>

class Menu : public Game
{
    public:
    bool runGame = false;

    private:
    World world;
    CameraComponent* camRef = nullptr;

    uint16_t backgroundColor = Color::BLUE;
    
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
    UIRenderSysetm myUIRenderSystem;
    UIUpdateSystem myUIUpdateSystem;

    public:
    Menu(GameManager& manager);
    ~Menu() = default;

    void Update(Input& input, float deltaTime) override;
    void Render(Renderer& renderer) override;
};