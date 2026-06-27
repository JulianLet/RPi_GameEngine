#include "Menu.h"

#include "Games/Menu/Systems/MenuFactory.h"

#include "Games/Pong/Pong.h"
#include "Games/JumpNRun/JumpNRun.h"
#include "Games/SDTest/SDTest.h"

#include <algorithm>

Menu::Menu(GameManager &manager) : Game("MENU", manager), myEventSystem(world, this)
{
    //reset
    runGame = false;

    for (uint8_t e = 0; e < MAX_ENTITIES; e++)
    {
        world.entities[e].isAlive = false;
        world.entities[e].mask = 0;
    }

    // --- World entites ---
    myCommonFactory.CreateStaticCamera(world, Vector2(65, 81), 1.f, 1.f);

    // --- UI entities ---
    myFactory.CreateMenuButton(
        world,
        Vector2(10,30),
        "PONG",
        [](GameManager& manager)
        {
            manager.SwitchGame(std::make_unique<Pong>(manager));
        });
        
    myFactory.CreateMenuButton(
        world,
        Vector2(10,60),
        "JUMP 'N' RUN",
        [](GameManager& manager)
        {
            manager.SwitchGame(std::make_unique<JumpNRun>(manager));
        });
            
    myFactory.CreateMenuButton(
        world,
        Vector2(10,90),
        "SD CARD",
        [](GameManager& manager)
        {
            manager.SwitchGame(std::make_unique<SDTest>(manager));
        });

    // --- init systems ---
    myUIButtonSystem.Initialize(world);
    myTilemapSystem.InitColliders(world);
}

void Menu::Update(Input &input, float deltaTime)
{
    myInputSystem.Update(world, input);
    myUIButtonSystem.Update(world, input, myGameManager);
    
    if (runGame)
    {
        myAISystem.Update(world, deltaTime);
        myCameraSystem.Update(world, deltaTime);
        
        myInputMoveSystem.Update(world, deltaTime);
        myMovementSystem.Update(world, deltaTime);
        myJumpSystem.Update(world, deltaTime);
        myFollowSystem.Update(world, deltaTime);
        
        myPhysicsSystem.Update(world, deltaTime);
        myCollisionSystem.Update(world);
        myPhysicsSystem.ResolveCollisions(world, deltaTime);
        
        myAnimationSystem.Update(world, deltaTime);
        myTimerSystem.Update(world, deltaTime);
        myUITimerSystem.Update(world);
    }
}

void Menu::Render(Renderer &renderer)
{
    renderer.Clear(backgroundColor);

    myTilemapSystem.Render(world, renderer);
    myShapeRenderSystem.Render(world, renderer);
    mySpriteRenderSystem.Render(world, renderer);
    myUIRenderSystem.Render(world, renderer);
}