#include "PongFactory.h"

uint8_t PongFactory::CreatePaddle(World &world, Vector2 startPos, KEYCODE up, KEYCODE down, bool isPlayer, uint8_t ballID)
{
    uint8_t id = world.CreateEntity();

    if (id == INVALID_ENTITY) return INVALID_ENTITY;

    world.entities[id].mask = TransformBit | MovementBit | RenderableBit | RectangleBit | PhysicsBit | ColliderBit | InputIntentBit;

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
        .layer = 0,
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
        mapping.directionMapping[KEYCODE::UP] = {InputAction::VERTICAL, -speed};
        mapping.directionMapping[KEYCODE::DOWN] = {InputAction::VERTICAL, speed};
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

    world.entities[id].mask = TransformBit | MovementBit | RenderableBit | RectangleBit | PhysicsBit | PhysicsMaterialBit | ColliderBit;// | CollisionResponseBit;

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
        .layer = 0,
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

    // the directional reflection
    // void Ball::OnCollisionEnter(Entity *self, Entity *other)
    // {
    //     if (other->tag == "Paddle") //add a bit of direction as well before it was 
    //     {
    //         TransformComponent* myTransform = GetComponent<TransformComponent>();
    //         TransformComponent* otherTransform = other->GetComponent<TransformComponent>();

    //         float deltaY = myTransform->GetCenterPos().y - otherTransform->GetCenterPos().y;

    //         //give vertical direction based on where on paddle it hit
    //         GetComponent<PhysicsComponent>()->currentVelocity.y = 2 * speed * deltaY / otherTransform->currentSize.y;
    //     }
    // }

    return id;
}

void PongFactory::PaddleThink(World &world, uint8_t self, float deltaTime)
{
    auto& mT = world.transforms[self];
    auto& myAI = world.ai[self];
    auto& bT = world.transforms[(int)myAI.memory[PongAI::BALL_ID]];
    auto& bP = world.physics[(int)myAI.memory[PongAI::BALL_ID]];

    float deltaX = (mT.currentPosition.x + mT.currentSize.x / 2) - (bT.currentPosition.x + bT.currentSize.x / 2); //rest way to travel for ball
    float estematedY = (bT.currentPosition.x + bT.currentSize.x / 2) + deltaX * bP.currentVelocity.Normalize().y; //calculated height of ball at paddel position

    myAI.memory[PongAI::ESTEMATED_Y] = estematedY;
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

    if (targetY < (mT.currentPosition.x + mT.currentSize.x / 2) - 10)
    {
        myAI.memory[PongAI::TARGET_DIR_Y] = -1; //move down
    }
    else if (targetY > (mT.currentPosition.x + mT.currentSize.x / 2) + 10)
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
    auto& myMovement = world.movements[self];
    auto& myAI = world.ai[self];

    myIntend.y = myAI.memory[PongAI::TARGET_DIR_Y] * myMovement.speed;
}
