#include "SDFactory.h"

uint8_t SDFactory::CreatePlayer(World &world, Vector2 startPos, Vector2 size, float moveSpeed)
{
    //load sprites
    uint8_t idle = world.assets.AddAnimation("idle.bin", 16, 16, 2, 4, AnimationMode::LOOP);
    uint8_t walk = world.assets.AddAnimation("walking.bin", 16, 16, 4, 4, AnimationMode::LOOP);

    uint8_t id = world.CreateEntity();

    if (id == INVALID_ENTITY) return INVALID_ENTITY;

    world.entities[id].mask = TransformBit | SpriteBit | AnimationBit | MovementBit | ColliderBit | PhysicsBit | RenderableBit | InputIntentBit | InputMappingBit;
    world.entities[id].tag = EntityTag::Player;

    world.transforms[id] =
    {
        .currentPosition = startPos,
        .lastPosition = startPos,
        .currentSize = size
    };

    world.sprites[id] =
    {

    };

    world.animations[id] =
    {
        .currentAnimation = idle,
        .currentFrame = 0,
        .currentTime = 0,
        .direction = 1,
        .active = true,
    };

    world.animSets[id].states[AnimState::Idle] = idle;
    world.animSets[id].states[AnimState::Walk] = walk;

    world.movements[id] =
    {
        .speed = moveSpeed
    };

    world.colliders[id] =
    {
        .offset = Vector2(0,0),
        .size = size,
        .isTrigger = SOLID
    };

    world.physics[id] =
    {
        .useGravity = NO_GRAVITY,
        .physicsType = PhysicsType::DYNAMIC, 
        .currentVelocity = Vector2(0,0),
    };

    world.renderables[id] =
    {
        .doRender = true,
        .layer = 5,
        .parallaxFactor = 1.0f
    };

    world.inputIntends[id] =
    {
        .hasX = true,
        .hasY = true
    };

    auto& mapping = world.inputMappings[id];
    mapping.directionMapping[KEYCODE::UP] = {InputAction::VERTICAL, -1};
    mapping.directionMapping[KEYCODE::DOWN] = {InputAction::VERTICAL, 1};
    mapping.directionMapping[KEYCODE::LEFT] = {InputAction::HORIZONTAL, -1};
    mapping.directionMapping[KEYCODE::RIGHT] = {InputAction::HORIZONTAL, 1};

    return id;
}

uint8_t SDFactory::CreateSpriteEntity(World &world, Vector2 pos, Vector2 size, const char *path, int index)
{   
    uint8_t spriteID = world.assets.AddSprite(path, size.x, size.y, index);

    uint8_t id = world.CreateEntity();

    if (id == INVALID_ENTITY) return INVALID_ENTITY;

    world.entities[id].mask = TransformBit | RenderableBit | SpriteBit;

    world.transforms[id] =
    {
        .currentPosition = pos,
        .lastPosition = pos,
        .currentSize = size
    };

    world.renderables[id] =
    {
        .doRender = true,
        .layer = 5,
        .parallaxFactor = 1
    };

    world.sprites[id] =
    {
        .spriteID = spriteID
    };

    return id;
}

uint8_t SDFactory::CreateTilemap(World &world)
{   
    world.assets.CreateTileset("TilesetTest.bin", 32, 32, 5);
    world.assets.myTileset.solidTiles[3] = true;

    // create the actual tilemap entity
    uint8_t id = world.CreateEntity();

    if (id == INVALID_ENTITY) return INVALID_ENTITY;

    world.entities[id].mask = TransformBit | TilemapBit;

    world.transforms[id] =
    {
        .currentPosition = Vector2(0, 0),
        .lastPosition = Vector2(0, 0),
        .currentSize = Vector2(320, 320)
    };

    auto& tm = world.tilemaps[id];
    tm.width = 10;
    tm.height = 10;

    // Load Tilemap File
    File tilemap = ResourceManager::GetInstance().sdManager.Open("Tilemap.txt", FA_READ);
    std::string text = tilemap.Read();
    tilemap.Close();

    for (char c : text)
    {
        if (c == '\n') continue;

        int index = c - '0'; //get index with ASCII

        tm.tileIndices.emplace_back(index); //fill with indices
    }

    return id;
}