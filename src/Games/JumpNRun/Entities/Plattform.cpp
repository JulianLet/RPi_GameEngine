#include "Plattform.h"

#include "Entities/Components/Core/TransformComponent.h"
#include "Entities/Components/Physics/ColliderComponent.h"
#include "Entities/Components/Physics/PhysicsComponent.h"
#include "Entities/Components/Render/RenderableComponent.h"
#include "Entities/Components/Render/RectangleComponent.h"

Plattform::Plattform(Vector2 pos, Vector2 size) : Entity("Ground")
{
    AddComponent<TransformComponent>(pos, size);
    AddComponent<ColliderComponent>(Vector2(0,0), size, STATIC, SOLID);
    AddComponent<PhysicsComponent>(NO_GRAVITY, Vector2(0,0), KINEMATIC);
    AddComponent<RenderableComponent>(1);
    AddComponent<RectangleComponent>(color, true);
}