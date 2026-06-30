#include "JumpNRun.h"

JumpNRun::JumpNRun(GameManager &manager) : Game("Jump 'N' Run", manager), myEventSystem(world, this), myActionSystem(this)
{
    ResetGame();
}

void JumpNRun::Update(Input &input, float deltaTime)
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

void JumpNRun::Render(Renderer &renderer)
{
    renderer.Clear(playingFieldColor);

    myTilemapSystem.Render(world, renderer);
    myShapeRenderSystem.Render(world, renderer);
    mySpriteRenderSystem.Render(world, renderer);
    myUIRenderSystem.Render(world, renderer);
}

void JumpNRun::ResetGame()
{
    //reset
    runGame = false;

    for (uint8_t e = 0; e < MAX_ENTITIES; e++)
    {
        world.entities[e].tag = EntityTag::Default;
        world.entities[e].isAlive = false;
        world.entities[e].mask = 0;
    }

    //new entities
    // --- World entities ---
    myCommonFactory.CreateStaticWall(world, Vector2(-50, 180), Vector2(210, 50), Color::BLACK); //ground
    myCommonFactory.CreateStaticWall(world, Vector2( 80, 150), Vector2(120, 30), Color::BLACK); //first step
    myCommonFactory.CreateStaticWall(world, Vector2(100, 110), Vector2(100, 40), Color::BLACK); //second step
    myCommonFactory.CreateStaticWall(world, Vector2(190,  30), Vector2( 10, 80), Color::BLACK); //wall right
    myCommonFactory.CreateStaticWall(world, Vector2(110,  30), Vector2( 50, 10), Color::BLACK); //air right
    myCommonFactory.CreateStaticWall(world, Vector2( 32,  45), Vector2( 30, 10), Color::BLACK); //air left
    myCommonFactory.CreateStaticWall(world, Vector2(-20, -30), Vector2( 10, 85), Color::BLACK); //wall left
    myCommonFactory.CreateStaticWall(world, Vector2( 20, -40), Vector2( 70, 10), Color::BLACK); //top
    myJRFactory.CreateGoal(world, Vector2( 75, -55), Vector2(10, 10));
    
    auto playerID = myCommonFactory.CreateSideScrollerPlayer(world, Vector2(0, 110), Vector2(8, 10), 50, Color::BLUE, 85);
    auto* playerTransform = &world.transforms[playerID];
    
    myCommonFactory.CrateFollowCammera(world, playerTransform, 1, 1, 30);

    for (int i = 0; i < 10; i++)
    {
        float randX = 5 + 20*i;
        float randY = 10;

        float randW = 20;
        float randH = 5;

        myJRFactory.CreateCloud(world, Vector2(randX, randY), Vector2(randW, randH));
    }

    // --- UI entities ---
    myCommonUIFactory.CreateUIText(world, Vector2(26, 8), "Jump 'N' Run", Color::BLACK, -1);
    myCommonUIFactory.CreateTimerText(world, Vector2(5,150), Color::WHITE, -1);

    // --- init systems ---
    myActionSystem.Initialize(playerID);
    myUIButtonSystem.Initialize(world);
    myTilemapSystem.InitColliders(world);
}
