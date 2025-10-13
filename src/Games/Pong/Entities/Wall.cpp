#include "Wall.h"

#include "Entities/Components/Core/TransformComponent.h"
#include "Entities/Components/Render/RenderableComponent.h"
#include "Entities/Components/Render/RectangleComponent.h"
#include "Entities/Components/Physics/ColliderComponent.h"
#include "Entities/Components/Physics/PhysicsComponent.h"
#include "Entities/Components/Physics/PhysicsMaterialComponent.h"

Wall::Wall(Vector2 pos, Vector2 size) : Entity("Wall")
{
    AddComponent<TransformComponent>(pos, size);
    AddComponent<RectangleComponent>(color, FILLED);
    AddComponent<RenderableComponent>(1);
    AddComponent<ColliderComponent>(Vector2{0,0}, size, STATIC, SOLID);
    AddComponent<PhysicsComponent>(NO_GRAVITY, Vector2{0,0}, KINEMATIC);
}