#include "SDFactory.h"

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
    // create sprites
    // create tileset
    // load sprites to cache <- temporary that should be done later

    // create the actual tilemap entity
    uint8_t id = world.CreateEntity();

    if (id == INVALID_ENTITY) return INVALID_ENTITY;



    return id;
}