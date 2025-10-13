#include "Cloud.h"

#include "Common.h"

#include "Entities/Components/Core/TransformComponent.h"
#include "Entities/Components/Render/RectangleComponent.h"
#include "Entities/Components/Render/RenderableComponent.h"

Cloud::Cloud(int x, int y)
{
    AddComponent<TransformComponent>(Vector2(x * 17, y), Vector2(15,10));
    AddComponent<RectangleComponent>(Color::WHITE, true);
    AddComponent<RenderableComponent>(x, true, 0.2f);
}