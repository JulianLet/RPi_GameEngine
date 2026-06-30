#include "SDTest.h"

SDTest::SDTest(GameManager &manager) : Game("SDTest", manager), myEventSystem(world, this)
{
    // auto& es = EventSystem::GetInstance();

    // // --- World entities ---
    // Entity* player = new TopDownPlayer(Vector2(40, 40), Vector2(5,10), 20.f, Color::RED);
    // auto* playerTransform = player->GetComponent<TransformComponent>();
    // es.DispatchEvent(EventSpawnEntity(player));

    // auto* camera = new FollowCamera(playerTransform, 1.f, 1.f, 30.f);
    // camRef = camera->GetComponent<CameraComponent>();
    // es.DispatchEvent(EventSpawnEntity(camera));

    //colored rec
    // auto* r = new Entity();
    // r->AddComponent<TransformComponent>(Vector2(10,10), Vector2(30,30));
    // r->AddComponent<RenderableComponent>(0);
    // r->AddComponent<RectangleComponent>(Color::RED, FILLED);
    // es.DispatchEvent(EventSpawnEntity(r));
    
    // auto* g = new Entity();
    // g->AddComponent<TransformComponent>(Vector2(50,10), Vector2(30,30));
    // g->AddComponent<RenderableComponent>(0);
    // g->AddComponent<RectangleComponent>(Color::GREEN, FILLED);
    // es.DispatchEvent(EventSpawnEntity(g));
    
    // auto* b = new Entity();
    // b->AddComponent<TransformComponent>(Vector2(90,10), Vector2(30,30));
    // b->AddComponent<RenderableComponent>(0);
    // b->AddComponent<RectangleComponent>(Color::BLUE, FILLED);
    // es.DispatchEvent(EventSpawnEntity(b));
    
    //colored blobs
    // auto* r = new Entity();
    // r->AddComponent<TransformComponent>(Vector2(0,0), Vector2(30,30));
    // r->AddComponent<RenderableComponent>(0);
    // r->AddComponent<SpriteComponent>(32, 32, "PngTest.bin", 0);
    // es.DispatchEvent(EventSpawnEntity(r));
    
    // auto* g = new Entity();
    // g->AddComponent<TransformComponent>(Vector2(35,0), Vector2(30,30));
    // g->AddComponent<RenderableComponent>(0);
    // g->AddComponent<SpriteComponent>(32, 32, "PngTest.bin", 1);
    // es.DispatchEvent(EventSpawnEntity(g));
    
    // auto* b = new Entity();
    // b->AddComponent<TransformComponent>(Vector2(70,0), Vector2(30,30));
    // b->AddComponent<RenderableComponent>(0);
    // b->AddComponent<SpriteComponent>(32, 32, "PngTest.bin", 2);
    // es.DispatchEvent(EventSpawnEntity(b));

    // Tileset tileset(32,32,5,"TilesetTest.bin");
    // tileset.solidTiles[3] = true;

    // Entity* tiles = new Entity();
    // tiles->AddComponent<TransformComponent>(Vector2(0,0), Vector2(320, 320)); //size of whole map
    // tiles->AddComponent<RenderableComponent>(0);
    // tiles->AddComponent<TilemapComponent>(10, 10, "Tilemap.txt", tileset);
    // es.DispatchEvent(EventSpawnEntity(tiles));
    
    // auto* bit = new Entity();
    // bit->AddComponent<TransformComponent>(Vector2(0,0), Vector2(225,225));
    // bit->AddComponent<RenderableComponent>(0);
    // bit->AddComponent<SpriteComponent>(225, 225, "8bitcolor.bin", 0);
    // es.DispatchEvent(EventSpawnEntity(bit));

    // --- UI elements ---
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
        
        myAnimationSystem.Update(world, deltaTime);
        myTimerSystem.Update(world, deltaTime);
        myUITimerSystem.Update(world);
    }
}

void SDTest::Render(Renderer &renderer)
{
    renderer.Clear(playingFieldColor);

    myTilemapSystem.Render(world, renderer);

    for (int layer = 0; layer < MAX_LAYERS; ++layer)
    {
        myShapeRenderSystem.Render(world, renderer, layer);
        mySpriteRenderSystem.Render(world, renderer, layer);
    }

    myUIRenderSystem.Render(world, renderer);
}