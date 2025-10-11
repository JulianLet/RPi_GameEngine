#include "Ball.h"

#include "Entities/Components/Transform.h"
#include "Entities/Components/Movement.h"
#include "Entities/Components/Renderable.h"
#include "Entities/Components/Rectangle.h"
#include "Entities/Components/Collider.h"
#include "Entities/Components/Physics.h"
#include "Entities/Components/PhysicsMaterial.h"
#include "Entities/Components/CollisionResponse.h"

void Ball::OnCollisionEnter(Entity *self, Entity *other)
{
    if (other->tag == "Paddle") //add a bit of direction as well before it was 
    {
        TransformComponent* myTransform = GetComponent<TransformComponent>();
        TransformComponent* otherTransform = other->GetComponent<TransformComponent>();

        float deltaY = myTransform->GetCenterPos().y - otherTransform->GetCenterPos().y;

        GetComponent<PhysicsComponent>()->currentVelocity.y = 2 * deltaY / otherTransform->currentSize.y;
        GetComponent<MovementComponent>()->currentSpeed++;
    }
}

Ball::Ball() : Entity("Ball")
{
    AddComponent<TransformComponent>(startPos, size);
    AddComponent<MovementComponent>(speed);
    AddComponent<RectangleComponent>(color, FILLED);
    AddComponent<ColliderComponent>(Vector2{0,0}, size, NOT_STATIC, SOLID);
    AddComponent<PhysicsComponent>(NO_GRAVITY, Vector2{-1, 0}, DYNAMIC);
    AddComponent<PhysicsMaterialComponent>(2.1f, 0.f);
    AddComponent<RenderableComponent>(0);
    
    AddComponent<CollisionResponseComponent>();
    GetComponent<CollisionResponseComponent>()->OnEnter = [this](Entity* self, Entity* other) 
    {
        this->OnCollisionEnter(self, other);
    };
}