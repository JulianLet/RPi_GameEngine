#include "JRGoal.h"

#include "Globals.h"

#include "Systems/Events/EventSystem.h"
#include "Systems/Events/Event.h"

#include "Entities/Components/Core/TransformComponent.h"
#include "Entities/Components/Physics/ColliderComponent.h"
#include "Entities/Components/Render/RectangleComponent.h"
#include "Entities/Components/Render/RenderableComponent.h"
#include "Entities/Components/Physics/CollisionResponseComponent.h"

void JRGoal::OnCollisionEnter(Entity* self, Entity* other)
{
    if (other->tag == "Player")
    {
        EventUpdateScore e (1, 0);
        EventSystem::GetInstance().DispatchEvent(e);
    }
}

JRGoal::JRGoal(Vector2 pos, Vector2 size)
{
    AddComponent<TransformComponent>(pos, size);
    AddComponent<ColliderComponent>(Vector2(0,0), size, TRIGGER);
    AddComponent<RectangleComponent>(Color::GREEN, true);
    AddComponent<CollisionResponseComponent>();
    AddComponent<RenderableComponent>(0);

    auto* response = GetComponent<CollisionResponseComponent>();
    response->OnEnter = [this](Entity* self, Entity* other)
    {
        OnCollisionEnter(self, other);
    };
}