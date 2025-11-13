#include "TopDownPlayer.h"

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

TopDownPlayer::TopDownPlayer(Vector2 startPos, Vector2 size, float moveSpeed, uint16_t color) : Entity("Player")
{

    AddComponent<TransformComponent>(startPos, size);
    AddComponent<MovementComponent>(moveSpeed);
    AddComponent<ColliderComponent>(Vector2(0,0), size, SOLID);
    AddComponent<PhysicsComponent>(NO_GRAVITY, Vector2(0,0), PhysicsType::DYNAMIC);
    AddComponent<RenderableComponent>(1);
    AddComponent<RectangleComponent>(color, true);

    AddComponent<InputIntendComponent>();
    auto* intend = GetComponent<InputIntendComponent>();
    intend->hasX = true;
    intend->hasY = true;

    //add inputs mapping
    AddComponent<InputMappingComponent>();
    auto* mapping = GetComponent<InputMappingComponent>();
    mapping->directionMapping[KEYCODE::UP] = {InputAction::VERTICAL, -moveSpeed};
    mapping->directionMapping[KEYCODE::DOWN] = {InputAction::VERTICAL, moveSpeed};
    mapping->directionMapping[KEYCODE::LEFT] = {InputAction::HORIZONTAL, -moveSpeed};
    mapping->directionMapping[KEYCODE::RIGHT] = {InputAction::HORIZONTAL, moveSpeed};
}