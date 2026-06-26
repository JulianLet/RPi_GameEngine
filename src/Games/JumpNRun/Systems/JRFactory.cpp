#include "JRFactory.h"

uint8_t JRFactory::CreateCloud(World &world, Vector2 pos, Vector2 size)
{
    uint8_t id = world.CreateEntity();

    if (id == INVALID_ENTITY) return INVALID_ENTITY;

    world.entities[id].mask = TransformBit | RenderableBit | RectangleBit;

    world.transforms[id] =
    {
        .currentPosition = pos,
        .lastPosition = pos,
        .currentSize = size
    };

    world.renderables[id] =
    {
        .doRender = true,
        .layer = 0,
        .parallaxFactor = 3.f,
    };

    world.rectangles[id] =
    {
        .currentColor = Color::WHITE,
        .filled = FILLED
    };

    return id;
}

uint8_t JRFactory::CreateGoal(World &world, Vector2 pos, Vector2 size)
{
    uint8_t id = world.CreateEntity();

    if (id == INVALID_ENTITY) return INVALID_ENTITY;

    world.entities[id].mask = TransformBit | RenderableBit | RectangleBit | ColliderBit | CollisionResponseBit;

    world.transforms[id] =
    {
        .currentPosition = pos,
        .lastPosition = pos,
        .currentSize = size
    };

    world.renderables[id] =
    {
        .doRender = true,
        .layer = 0,
        .parallaxFactor = 1.f,
    };

    world.rectangles[id] =
    {
        .currentColor = Color::GREEN,
        .filled = FILLED
    };

    world.colliders[id] =
    {
        .offset = Vector2(0, 0),
        .size = size,
        .isTrigger = TRIGGER,
    };

    world.collisionResponses[id] =
    {
        .OnEnter = [this](World& world, uint8_t self, uint8_t other)
        {
            if (world.entities[other].tag == EntityTag::Player)
            {
                EventUpdateScore e (1, 0);
                EventManager::GetInstance().DispatchEvent(e);
            }
        }
    };
    
    return id;
}
