#include "Entities/Components/FollowTarget.h"
#include "Entities/Components/Transform.h"

FollowTargetComponent::~FollowTargetComponent() = default;

FollowTargetComponent::FollowTargetComponent(TransformComponent *target)
    : target(target)
{

}

void FollowTargetComponent::Reset()
{

}
