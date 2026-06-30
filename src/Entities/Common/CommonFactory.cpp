#include "CommonFactory.h"

#include "Managers/Game/World.h"

uint8_t CommonFactory::CreateTopDownPlayer(World &world, Vector2 startPos, Vector2 size, float moveSpeed, uint8_t color)
{
    uint8_t id = world.CreateEntity();

    if (id == INVALID_ENTITY) return INVALID_ENTITY;

    world.entities[id].mask = TransformBit | RectangleBit | MovementBit | ColliderBit | PhysicsBit | RenderableBit | InputIntentBit | InputMappingBit;
    world.entities[id].tag = EntityTag::Player;

    world.transforms[id] =
    {
        .currentPosition = startPos,
        .lastPosition = startPos,
        .currentSize = size
    };

    world.rectangles[id] =
    {
        .currentColor = color,
        .filled = FILLED
    };

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
        .layer = 1,
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

    /* ANIMATION:
    AddComponent<AnimationComponent>(16, 16);
    auto* animation = GetComponent<AnimationComponent>();
    animation->AddAnimation(0, AnimationMode::LOOP, 2, 1, "idle.bin");
    animation->AddAnimation(1, AnimationMode::LOOP, 4, 4, "walking.bin");

    animation->transitions[0].push_back([this](bool& swapped) 
    {
        auto* physics = GetComponent<PhysicsComponent>();

        if (physics->currentVelocity.Magnitude() > 0.2f)
        {
            GetComponent<AnimationComponent>()->SwapAnimation(1);
            swapped = true;
        }
    });

    animation->transitions[1].push_back([this](bool& swapped) 
    {
        auto* physics = GetComponent<PhysicsComponent>();

        if (physics->currentVelocity.Magnitude() < 0.2f)
        {
            GetComponent<AnimationComponent>()->SwapAnimation(0);
            swapped = true;
        }
    });
    */

    return id;
}

uint8_t CommonFactory::CreateSideScrollerPlayer(World& world, Vector2 startPos, Vector2 size, float moveSpeed, uint8_t color, float jumpPower)
{
    uint8_t id = world.CreateEntity();

    if (id == INVALID_ENTITY) return INVALID_ENTITY;

    world.entities[id].mask = TransformBit | MovementBit | JumpBit | RectangleBit | RenderableBit | ColliderBit | PhysicsBit | InputIntentBit | InputMappingBit; // | CollisionResponseBit | OnEventBit;
    world.entities[id].tag = EntityTag::Player;

    world.transforms[id] =
    {
        .currentPosition = startPos,
        .lastPosition = startPos,
        .currentSize = size
    };

    world.movements[id] =
    {
        .speed = moveSpeed
    };

    world.jumps[id] =
    {
        .jumpPower = jumpPower
    };

    world.rectangles[id] =
    {
        .currentColor = color,
        .filled = FILLED
    };

    world.renderables[id] =
    {
        .doRender = true,
        .layer = 1,
        .parallaxFactor = 1.0f
    };

    world.colliders[id] =
    {
        .offset = Vector2(0,0),
        .size = size,
        .isTrigger = SOLID
    };

    world.physics[id] =
    {
        .useGravity = USE_GRAVITY,
        .physicsType = PhysicsType::DYNAMIC, 
        .currentVelocity = Vector2(0,0),
    };

    world.inputIntends[id] =
    {
        .hasX = true,
        .hasY = false
    };

    auto& mapping = world.inputMappings[id];
    mapping.directionMapping[KEYCODE::LEFT] = {InputAction::HORIZONTAL, -1};
    mapping.directionMapping[KEYCODE::RIGHT] = {InputAction::HORIZONTAL, 1};
    mapping.actionMapping[KEYCODE::A] = {InputAction::JUMP};

    return id;
}

uint8_t CommonFactory::CreateStaticCamera(World& world, Vector2 position, float initZoom, float targetZoom)
{
    uint8_t id = world.CreateEntity();

    if (id == INVALID_ENTITY) return INVALID_ENTITY;

    world.activeCamera = id;
    world.entities[id].mask = TransformBit | CameraBit;

    world.transforms[id] =
    {
        .currentPosition = position,
        .lastPosition = position,
        .currentSize = Vector2(0,0)
    };

    world.cameras[id] =
    {
        .currentZoom = initZoom,
        .targetZoom = targetZoom,
        .zoomSpeed = 2.f
    };

    return id;
}

uint8_t CommonFactory::CrateFollowCammera(World& world, TransformComponent* target, float initZoom, float targetZoom, float speed)
{
    uint8_t id = world.CreateEntity();

    if (id == INVALID_ENTITY) return INVALID_ENTITY;

    world.activeCamera = id;
    world.entities[id].mask = TransformBit | CameraBit | MovementBit | FollowTargetBit;

    world.transforms[id] =
    {
        .currentPosition = target->currentPosition,
        .lastPosition = target->currentPosition,
        .currentSize = Vector2(0,0)
    };

    world.cameras[id] =
    {
        .currentZoom = initZoom,
        .targetZoom = targetZoom,
        .zoomSpeed = 2.f
    };

    world.movements[id] = 
    {
        .speed = speed
    };

    world.followTargets[id] =
    {
        .target = target
    };

    return id;
}

uint8_t CommonFactory::CreateStaticWall(World& world, Vector2 pos, Vector2 size, uint8_t color)
{
    uint8_t id = world.CreateEntity();

    if (id == INVALID_ENTITY) return INVALID_ENTITY;

    world.entities[id].mask = TransformBit | RectangleBit | RenderableBit | ColliderBit | PhysicsBit;
    world.entities[id].tag = EntityTag::Ground;

    world.transforms[id] =
    {
        .currentPosition = pos,
        .lastPosition = pos,
        .currentSize = size
    };

    world.rectangles[id] =
    {
        .currentColor = color,
        .filled = FILLED
    };

    world.renderables[id] =
    {
        .doRender = true,
        .layer = 0,
        .parallaxFactor = 1.f
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
        .physicsType = PhysicsType::STATIC,
        .currentVelocity = Vector2(0,0)
    };

    return id;
}
