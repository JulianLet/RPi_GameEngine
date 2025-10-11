#include "Wall.h"

#include "Entities/Components/Transform.h"
#include "Entities/Components/Renderable.h"
#include "Entities/Components/Rectangle.h"
#include "Entities/Components/Collider.h"
#include "Entities/Components/Physics.h"
#include "Entities/Components/PhysicsMaterial.h"

Wall::Wall(Vector2 pos, Vector2 size) : Entity("Wall")
{
    AddComponent<TransformComponent>(pos, size);
    AddComponent<RectangleComponent>(color, FILLED);
    AddComponent<RenderableComponent>(1);
    AddComponent<ColliderComponent>(Vector2{0,0}, size, STATIC, SOLID);
    AddComponent<PhysicsComponent>(NO_GRAVITY, Vector2{0,0}, KINEMATIC);
}