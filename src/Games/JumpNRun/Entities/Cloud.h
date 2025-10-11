#pragma once

#include "Entities/Entity.h"
#include "Globals.h"

class Cloud : public Entity
{
private:
    uint16_t color = Color::BLACK;

    public:
    Cloud(int x, int y);
    ~Cloud() = default;
};