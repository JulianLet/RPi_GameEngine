#pragma once

#include "Entities/Component.h"

struct PhysicsMaterialComponent : public Component
{
    float friction = 0; //oncollision stay
    float bounciness = 0; //on collision enter

    PhysicsMaterialComponent(float bounciness, float friction) : bounciness(bounciness), friction(friction) {};
    ~PhysicsMaterialComponent() override;

    void Reset() override;
};