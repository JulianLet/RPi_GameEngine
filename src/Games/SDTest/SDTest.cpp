#include "SDTest.h"

SDTest::SDTest(GameManager &manager) : Game("SDTest", manager), myEventSystem(world, this)
{
    runGame = false;

    for (uint8_t e = 0; e < MAX_ENTITIES; e++)
    {
        world.entities[e].tag = EntityTag::Default;
        world.entities[e].isAlive = false;
        world.entities[e].mask = 0;
    }

    // new entities
    // --- World entities ---
    mySDFactory.CreateTilemap(world);
    
    auto playerID = mySDFactory.CreatePlayer(world, Vector2(40,40), Vector2(16,16), 30);
    auto* pT = &world.transforms[playerID];
    
    myCommonFactory.CrateFollowCammera(world, pT, 1, 1, 40);
    

    // --- sprite test
    // myCommonFactory.CreateStaticCamera(world, Vector2(50,70), 1, 1);
    // mySDFactory.CreateSpriteEntity(world, Vector2(10,40), Vector2(16,16), "colors.bin", 0);   
    // mySDFactory.CreateSpriteEntity(world, Vector2(30,40), Vector2(16,16), "colors.bin", 1);   
    // mySDFactory.CreateSpriteEntity(world, Vector2(50,40), Vector2(16,16), "colors.bin", 2);   

    // mySDFactory.CreateSpriteEntity(world, Vector2(00,60), Vector2(32, 32), "TilesetTest.bin", 0);   
    // mySDFactory.CreateSpriteEntity(world, Vector2(35,60), Vector2(32, 32), "TilesetTest.bin", 1);   
    // mySDFactory.CreateSpriteEntity(world, Vector2(70,60), Vector2(32, 32), "TilesetTest.bin", 2);   
    // mySDFactory.CreateSpriteEntity(world, Vector2(0,95), Vector2(32, 32), "TilesetTest.bin", 3);   
    // mySDFactory.CreateSpriteEntity(world, Vector2(35,95), Vector2(32, 32), "TilesetTest.bin", 4);  
     
    // --- color test
    // myCommonFactory.CreateStaticCamera(world, Vector2(50,70), 1, 1);
    // myCommonFactory.CreateStaticWall(world, Vector2(30,30), Vector2(30,80), Color::RED);
    // myCommonFactory.CreateStaticWall(world, Vector2(60,30), Vector2(30,80), Color::GREEN);
    // myCommonFactory.CreateStaticWall(world, Vector2(90,30), Vector2(30,80), Color::BLUE);

    // --- UI elements ---
    
    
    // --- init systems ---
    myUIButtonSystem.Initialize(world);
    myTilemapSystem.InitColliders(world);

    runGame = true;
}

void SDTest::Update(Input &input, float deltaTime)
{
    myInputSystem.Update(world, input);
    //myActionSystem.Update(world, input); 
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
        
        myPlayerAnimSystem.Update(world);
        myAnimationSystem.Update(world, deltaTime);
        myTimerSystem.Update(world, deltaTime);
        myUITimerSystem.Update(world);
    }
}

void SDTest::Render(Renderer &renderer)
{
    renderer.Clear(backgroundColor);

    myTilemapSystem.Render(world, renderer);

    for (int layer = 0; layer < MAX_LAYERS; ++layer)
    {
        myShapeRenderSystem.Render(world, renderer, layer);
        mySpriteRenderSystem.Render(world, renderer, layer);
    }

    myUIRenderSystem.Render(world, renderer);
}