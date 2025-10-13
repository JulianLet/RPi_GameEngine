#include "Player.h"

#include "Systems/Events/Event.h"

#include "Entities/Components/Core/TransformComponent.h"
#include "Entities/Components/Core/MovementComponent.h"
#include "Entities/Components/Physics/ColliderComponent.h"
#include "Entities/Components/Physics/CollisionResponseComponent.h"
#include "Entities/Components/Physics/PhysicsComponent.h"
#include "Entities/Components/Input/InputIntendComponent.h"
#include "Entities/Components/Input/InputMappingComponent.h"
#include "Entities/Components/Render/RectangleComponent.h"
#include "Entities/Components/Render/RenderableComponent.h"
#include "Entities/Components/Events/OnEventComponent.h"

Player::Player(Vector2 startPos) : Entity("Player")
{
    AddComponent<TransformComponent>(startPos, size);
    AddComponent<MovementComponent>(speed);
    AddComponent<ColliderComponent>(Vector2(0,0), size, NOT_STATIC, SOLID);
    AddComponent<PhysicsComponent>(USE_GRAVITY, Vector2(0,0), DYNAMIC);
    AddComponent<RenderableComponent>(1);
    AddComponent<RectangleComponent>(color, true);
    AddComponent<InputIntendComponent>();
    auto* intend = GetComponent<InputIntendComponent>();
    intend->hasX = true;

    //add inputs mapping
    AddComponent<InputMappingComponent>();
    auto* mapping = GetComponent<InputMappingComponent>();
    mapping->directionMapping[KEYCODE::LEFT] = {InputAction::HORIZONTAL, -1};
    mapping->directionMapping[KEYCODE::RIGHT] = {InputAction::HORIZONTAL, 1};
    mapping->actionMapping[KEYCODE::A] = {InputAction::JUMP};
    
    //add collider responses
    AddComponent<CollisionResponseComponent>();
    auto* response = GetComponent<CollisionResponseComponent>();
    response->OnEnter = [this](Entity* self, Entity* other) 
    {
        this->OnCollisionEnter(self, other);
    };

    response->OnExit = [this](Entity* self, Entity* other) 
    {
        this->OnCollisionExit(self, other);
    };


    //add events
    AddComponent<OnEventComponent>();

    auto* onEvent = GetComponent<OnEventComponent>();
    onEvent->events[EventType::JUMP_EVENT] = [this](const Event &event) 
    {
        this->Jump(event);
    };
}

void Player::OnCollisionEnter(Entity* self, Entity* other)
{
    if (other->tag == "Ground")
    {
        isGrounded = true;
        auto* physics = self->GetComponent<PhysicsComponent>();

        physics->useGravity = false;
        physics->currentVelocity.y = 0;
    }
}

void Player::OnCollisionExit(Entity* self, Entity* other)
{
    if (other->tag == "Ground")
    {
        auto* physics = self->GetComponent<PhysicsComponent>();
        physics->useGravity = true;
    }
}

void Player::Jump(const Event &event)
{
        if (!isGrounded) return;
        isGrounded = false;

        auto* physics = GetComponent<PhysicsComponent>();
        physics->currentVelocity.y = -3.f;
}