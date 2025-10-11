#include "Cloud.h"

#include "Common.h"

#include "Entities/Components/Transform.h"
#include "Entities/Components/Rectangle.h"
#include "Entities/Components/Renderable.h"

Cloud::Cloud(int x, int y)
{
    AddComponent<TransformComponent>(Vector2(x * 17, y), Vector2(15,10));
    AddComponent<RectangleComponent>(Color::WHITE, true);
    AddComponent<RenderableComponent>(x, true, 0.2f);
}