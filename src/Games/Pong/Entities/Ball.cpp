#include "Ball.h"

#include "Entities/Components/Core/TransformComponent.h"
#include "Entities/Components/Core/MovementComponent.h"

#include "Entities/Components/Render/RenderableComponent.h"
#include "Entities/Components/Render/RectangleComponent.h"

#include "Entities/Components/Physics/ColliderComponent.h"
#include "Entities/Components/Physics/PhysicsComponent.h"
#include "Entities/Components/Physics/PhysicsMaterialComponent.h"
#include "Entities/Components/Physics/CollisionResponseComponent.h"

void Ball::OnCollisionEnter(Entity *self, Entity *other)
{
    if (other->tag == "Paddle") //add a bit of direction as well before it was 
    {
        TransformComponent* myTransform = GetComponent<TransformComponent>();
        TransformComponent* otherTransform = other->GetComponent<TransformComponent>();

        float deltaY = myTransform->GetCenterPos().y - otherTransform->GetCenterPos().y;

        //give vertical direction based on where on paddle it hit
        GetComponent<PhysicsComponent>()->currentVelocity.y = 2 * speed * deltaY / otherTransform->currentSize.y;
    }
}

Ball::Ball() : Entity("Ball")
{
    AddComponent<TransformComponent>(startPos, size);
    AddComponent<MovementComponent>(speed);
    AddComponent<RectangleComponent>(color, FILLED);
    AddComponent<ColliderComponent>(Vector2{0,0}, size, SOLID);
    AddComponent<PhysicsComponent>(NO_GRAVITY, Vector2{-speed, 0}, PhysicsType::DYNAMIC);
    AddComponent<PhysicsMaterialComponent>(2.2f, 0.f);
    AddComponent<RenderableComponent>(0);
    
    AddComponent<CollisionResponseComponent>();
    GetComponent<CollisionResponseComponent>()->OnEnter = [this](Entity* self, Entity* other) 
    {
        this->OnCollisionEnter(self, other);
    };
}