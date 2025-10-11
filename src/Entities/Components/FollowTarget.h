#pragma once

#include "Entities/Component.h"

class TransformComponent;

struct FollowTargetComponent : public Component
{
    TransformComponent* target;
    
    FollowTargetComponent(TransformComponent* target);
    ~FollowTargetComponent() override;

    void Reset() override;
};