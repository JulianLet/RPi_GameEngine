#include "Paddle.h"

#include "Games/Pong/Pong.h"

#include "Entities/Components/Core/TransformComponent.h"
#include "Entities/Components/Core/MovementComponent.h"

#include "Entities/Components/Render/RenderableComponent.h"
#include "Entities/Components/Render/RectangleComponent.h"

#include "Entities/Components/Physics/ColliderComponent.h"
#include "Entities/Components/Physics/PhysicsComponent.h"
#include "Entities/Components/Physics/PhysicsMaterialComponent.h"
#include "Entities/Components/Physics/CollisionResponseComponent.h"

#include "Entities/Components/AI/AIComponent.h"
#include "Entities/Components/Input/InputIntendComponent.h"
#include "Entities/Components/Input/InputMappingComponent.h"

void Paddle::OnCollisionEnter(Entity* self, Entity* other)
{
}

Paddle::Paddle(Pong* pongRef, Vector2 startPos, KEYCODE up, KEYCODE down, bool isPlayer) : Entity("Paddle")
{
    Paddle::pongRef = pongRef;
    Paddle::isPlayer = isPlayer;

    AddComponent<TransformComponent>(startPos, size);
    AddComponent<MovementComponent>(speed);

    AddComponent<PhysicsComponent>(NO_GRAVITY, Vector2{0,0}, KINEMATIC);
    AddComponent<ColliderComponent>(Vector2{0,0}, size, NOT_STATIC, SOLID);

    AddComponent<RenderableComponent>(0);
    AddComponent<RectangleComponent>(color, FILLED);
    AddComponent<InputIntendComponent>();
    auto* intend = GetComponent<InputIntendComponent>();
    intend->hasY = true;

    if (isPlayer)
    {
        AddComponent<InputMappingComponent>();

        auto* mapping = GetComponent<InputMappingComponent>();
        mapping->directionMapping[KEYCODE::UP] = {InputAction::VERTICAL, -1};
        mapping->directionMapping[KEYCODE::DOWN] = {InputAction::VERTICAL, 1};
    }
    else
    {
        AddComponent<AIComponent>();

        auto* ai = GetComponent<AIComponent>();
        ai->think = [this](Entity* self, float deltaTime)
        {
            this->Think(self, deltaTime);
        };

        
        ai->decide = [this](Entity* self, float deltaTime)
        {
            this->Decide(self, deltaTime);
        };

        
        ai->act = [this](Entity* self, float deltaTime)
        {
            this->Act(self, deltaTime);
        };
    }
}

namespace PongAI
{
    enum Momory {ESTEMATED_Y = 0, BALL_TO_RIGHT, TARGET_DIR_Y};
}

void Paddle::Think(Entity *self, float deltaTime)
{
    if (!myTransform) myTransform = GetComponent<TransformComponent>();
    if (!myIntend) myIntend = GetComponent<InputIntendComponent>();
    if (!ballTransform) ballTransform = pongRef->GetBall()->GetComponent<TransformComponent>();
    if (!ballPhysics) ballPhysics = pongRef->GetBall()->GetComponent<PhysicsComponent>();
    if (!myAI) myAI = GetComponent<AIComponent>();

    float deltaX = myTransform->GetCenterPos().x - ballTransform->GetCenterPos().x; //rest way to travel for ball
    float estematedY = ballTransform->GetCenterPos().y + deltaX * ballPhysics->currentVelocity.y; //calculated height of ball at paddel position

    myAI->memory[PongAI::ESTEMATED_Y] = estematedY;
    myAI->memory[PongAI::BALL_TO_RIGHT] = ballPhysics->currentVelocity.x > 0 ? 1 : -1;
}

void Paddle::Decide(Entity *self, float deltaTime)
{
    if (!myAI) return;

    float targetY = 80; //default middle of screen

    if (myAI->memory[PongAI::BALL_TO_RIGHT] > 0)
    {
        targetY = myAI->memory[PongAI::ESTEMATED_Y];
    }

    if (targetY < myTransform->GetCenterPos().y - 10)
    {
        myAI->memory[PongAI::TARGET_DIR_Y] = -1; //move down
    }
    else if (targetY > myTransform->GetCenterPos().y + 10)
    {
        myAI->memory[PongAI::TARGET_DIR_Y] = 1;
    }
    else
    {
        myAI->memory[PongAI::TARGET_DIR_Y] = 0;
    }
}

void Paddle::Act(Entity *self, float deltaTime)
{
    if (!myIntend || !myAI) return;

    myIntend->y = myAI->memory[PongAI::TARGET_DIR_Y];
}
