#include "PongFactory.h"

uint8_t PongFactory::CreatePaddle(World &world, Vector2 startPos, KEYCODE up, KEYCODE down, bool isPlayer, uint8_t ballID)
{
    uint8_t id = world.CreateEntity();

    if (id == INVALID_ENTITY) return INVALID_ENTITY;

    world.entities[id].mask = TransformBit | MovementBit | RenderableBit | RectangleBit | PhysicsBit | ColliderBit | InputIntentBit;
    world.entities[id].tag = EntityTag::Player;

    Vector2 size = Vector2(5, 30);
    float speed = 40.f;

    world.transforms[id] =
    {
        .currentPosition = startPos,
        .lastPosition = startPos,
        .currentSize = size
    };

    world.movements[id] =
    {
        .speed = speed
    };

    world.renderables[id] =
    {
        .doRender = true,
        .layer = 5,
        .parallaxFactor = 1.f,
    };

    world.rectangles[id] =
    {
        .currentColor = Color::BLUE,
        .filled = FILLED
    };

    world.physics[id] =
    {
        .useGravity = false,
        .physicsType = PhysicsType::KINEMATIC,
        .currentVelocity = Vector2(0, 0),
    };

    world.colliders[id] =
    {
        .offset = Vector2(0, 0),
        .size = size,
        .isTrigger = SOLID,
    };

    world.inputIntends[id] = 
    {
        .hasX = false,
        .hasY = true
    };


    if (isPlayer)
    {
        world.entities[id].mask |= InputMappingBit;

        auto& mapping = world.inputMappings[id];
        mapping.directionMapping[KEYCODE::UP] = {InputAction::VERTICAL, -1};
        mapping.directionMapping[KEYCODE::DOWN] = {InputAction::VERTICAL, 1};
    }
    else
    {
        world.entities[id].mask |= AIBit;

        auto& ai = world.ai[id];
        ai.memory[PongAI::BALL_ID] = ballID;

        ai.think = [this](World& world, uint8_t self, float deltaTime)
        {
            this->PaddleThink(world, self, deltaTime);
        };

        
        ai.decide = [this](World& world, uint8_t self, float deltaTime)
        {
            this->PaddleDecide(world, self, deltaTime);
        };

        
        ai.act = [this](World& world, uint8_t self, float deltaTime)
        {
            this->PaddleAct(world, self, deltaTime);
        };
    }

    return id;
}

uint8_t PongFactory::CreateBall(World &world)
{
    uint8_t id = world.CreateEntity();

    if (id == INVALID_ENTITY) return INVALID_ENTITY;

    world.entities[id].mask = TransformBit | MovementBit | RenderableBit | RectangleBit | PhysicsBit | PhysicsMaterialBit | ColliderBit | CollisionResponseBit;
    world.entities[id].tag = EntityTag::Ball;

    Vector2 startPos = {64,80};
    Vector2 size = {3,3};
    float speed = 40.f;

    world.transforms[id] =
    {
        .currentPosition = startPos,
        .lastPosition = startPos,
        .currentSize = size
    };

    world.movements[id] =
    {
        .speed = speed
    };

    world.renderables[id] =
    {
        .doRender = true,
        .layer = 5,
        .parallaxFactor = 1.f,
    };

    world.rectangles[id] =
    {
        .currentColor = Color::WHITE,
        .filled = FILLED
    };

    world.physics[id] =
    {
        .useGravity = false,
        .physicsType = PhysicsType::DYNAMIC,
        .currentVelocity = Vector2(-speed, 0),
    };

    world.physicsMaterials[id] =
    {
        .friction = 0,
        .bounciness = 2.2f,
    };

    world.colliders[id] =
    {
        .offset = Vector2(0, 0),
        .size = size,
        .isTrigger = SOLID,
    };

    // directional reflection on paddle hit
    auto& response = world.collisionResponses[id];
    response.OnEnter = [](World& world, uint8_t self, uint8_t other)
    {
        if (world.entities[other].tag == EntityTag::Player)
        {
            auto& mT = world.transforms[self];
            auto& mP = world.physics[self];
            auto& oT = world.transforms[other];

            float myCenter = mT.currentPosition.y + mT.currentSize.y * 0.5f;
            float otherCenter = oT.currentPosition.y + oT.currentSize.y * 0.5f;

            float deltaY = (myCenter - otherCenter) / (oT.currentSize.y * 0.5f);

            // preserve speed
            float speed = mP.currentVelocity.Magnitude();

            mP.currentVelocity.y = deltaY * speed;
        }
    };

    return id;
}

void PongFactory::PaddleThink(World &world, uint8_t self, float deltaTime)
{
    auto& mT = world.transforms[self];
    auto& myAI = world.ai[self];
    auto& bT = world.transforms[(int)myAI.memory[PongAI::BALL_ID]];
    auto& bP = world.physics[(int)myAI.memory[PongAI::BALL_ID]];

    float ballX = bT.currentPosition.x + (bT.currentSize.x / 2);
    float paddleX = mT.currentPosition.x + (mT.currentSize.x / 2);

    float ballY = bT.currentPosition.y + (bT.currentSize.y / 2);

    float deltaX = paddleX - ballX; //rest way to travel for ball
    float estimatedY = ballY + deltaX * bP.currentVelocity.Normalize().y; //calculated height of ball at paddel position

    myAI.memory[PongAI::ESTEMATED_Y] = estimatedY;
    myAI.memory[PongAI::BALL_TO_RIGHT] = bP.currentVelocity.x > 0 ? 1 : -1;
}

void PongFactory::PaddleDecide(World &world, uint8_t self, float deltaTime)
{
    auto& mT = world.transforms[self];
    auto& myAI = world.ai[self];
    float targetY = 80; //default middle of screen

    if (myAI.memory[PongAI::BALL_TO_RIGHT] > 0)
    {
        targetY = myAI.memory[PongAI::ESTEMATED_Y];
    }

    float paddleCenterY = mT.currentPosition.y + mT.currentSize.y / 2;

    if (targetY < paddleCenterY - 10)
    {        
        myAI.memory[PongAI::TARGET_DIR_Y] = -1;
    }    
    else if (targetY > paddleCenterY + 10)
    {        
        myAI.memory[PongAI::TARGET_DIR_Y] = 1;
    }    
    else
    {        
        myAI.memory[PongAI::TARGET_DIR_Y] = 0;
    }
}

void PongFactory::PaddleAct(World &world, uint8_t self, float deltaTime)
{
    auto& myIntend = world.inputIntends[self];
    auto& myAI = world.ai[self];

    myIntend.y = myAI.memory[PongAI::TARGET_DIR_Y];
}
