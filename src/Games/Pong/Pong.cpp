#include "Pong.h"


Pong::Pong(GameManager &manager) 
    : Game("PONG", manager), myEventSystem(world, this), myActionSystem(this)
{
    ResetGame();
}

void Pong::Update(Input &input, float deltaTime)
{
    myInputSystem.Update(world, input);
    myActionSystem.Update(world, input); 
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

void Pong::Render(Renderer &renderer)
{
    renderer.Clear(playingFieldColor);

    myTilemapSystem.Render(world, renderer);
    myShapeRenderSystem.Render(world, renderer);
    mySpriteRenderSystem.Render(world, renderer);
    myUIRenderSystem.Render(world, renderer);
}

void Pong::ResetGame()
{
    //reset
    runGame = false;

    for (uint8_t e = 0; e < MAX_ENTITIES; e++)
    {
        world.entities[e].isAlive = false;
        world.entities[e].mask = 0;
    }

    //new entities
    myCommonFactory.CreateStaticCamera(world, Vector2(65, 81), 1.f, 1.f);

    // --- World entities ---
    uint8_t ballID = myPongFactory.CreateBall(world);
    myPongFactory.CreatePaddle(world, Vector2(5, 65), KEYCODE::UP, KEYCODE::DOWN, true, ballID);
    myPongFactory.CreatePaddle(world, Vector2(120, 65), KEYCODE::LEFT, KEYCODE::RIGHT, false, ballID);
    myCommonFactory.CreateStaticWall(world, Vector2(0, 0), Vector2(playingFieldSize.x, 30), Color::BLACK, "Wall");
    myCommonFactory.CreateStaticWall(world, Vector2(0, playingFieldSize.y + 30), Vector2(playingFieldSize.x, 35), Color::BLACK, "Wall");

    // --- UI entities ---
    myCommonUIFactory.CreateUIText(world, Vector2(57, 8), "PONG", Color::WHITE, 0); 
    uint8_t scoreID = myCommonUIFactory.CreateUIText(world, Vector2(65, 18), "SCORE TEXT", Color::WHITE, 0); 
    myCommonUIFactory.CreateUIText(world, Vector2(80, 150), "by Julez", Color::WHITE, 0); 

    // --- init systems ---
    myUIButtonSystem.Initialize(world);
    myTilemapSystem.InitColliders(world);
    myActionSystem.Initialize(world, ballID, scoreID);
}