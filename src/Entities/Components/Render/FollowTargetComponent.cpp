#include "FollowTargetComponent.h"
#include "Entities/Components/Core/TransformComponent.h"

FollowTargetComponent::~FollowTargetComponent() = default;

FollowTargetComponent::FollowTargetComponent(TransformComponent *target)
    : target(target)
{

}

void FollowTargetComponent::Reset()
{

}
