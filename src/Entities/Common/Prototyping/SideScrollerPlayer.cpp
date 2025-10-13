#include "SideScrollerPlayer.h"

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

SideScrollerPlayer::SideScrollerPlayer(Vector2 startPos, Vector2 size, float moveSpeed, uint16_t color, float jumpPower) : Entity("Player"), jumpPower(jumpPower)
{

    AddComponent<TransformComponent>(startPos, size);
    AddComponent<MovementComponent>(moveSpeed);
    AddComponent<ColliderComponent>(Vector2(0,0), size, SOLID);
    AddComponent<PhysicsComponent>(USE_GRAVITY, Vector2(0,0), PhysicsType::DYNAMIC);
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

void SideScrollerPlayer::OnCollisionEnter(Entity* self, Entity* other)
{
    if (other->tag == "Ground")
    {
        isGrounded = true;
        auto* physics = self->GetComponent<PhysicsComponent>();

        physics->useGravity = false;
        physics->currentVelocity.y = 0;
    }
}

void SideScrollerPlayer::OnCollisionExit(Entity* self, Entity* other)
{
    if (other->tag == "Ground")
    {
        auto* physics = self->GetComponent<PhysicsComponent>();
        physics->useGravity = true;
    }
}

void SideScrollerPlayer::Jump(const Event &event)
{
        if (!isGrounded) return;
        isGrounded = false;

        auto* physics = GetComponent<PhysicsComponent>();
        physics->currentVelocity.y = -jumpPower;
}