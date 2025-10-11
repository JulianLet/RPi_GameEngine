#include "Plattform.h"

#include "Entities/Components/Transform.h"
#include "Entities/Components/Collider.h"
#include "Entities/Components/Physics.h"
#include "Entities/Components/Renderable.h"
#include "Entities/Components/Rectangle.h"

Plattform::Plattform(Vector2 pos, Vector2 size) : Entity("Ground")
{
    AddComponent<TransformComponent>(pos, size);
    AddComponent<ColliderComponent>(Vector2(0,0), size, STATIC, SOLID);
    AddComponent<PhysicsComponent>(NO_GRAVITY, Vector2(0,0), KINEMATIC);
    AddComponent<RenderableComponent>(1);
    AddComponent<RectangleComponent>(color, true);
}