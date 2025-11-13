#pragma once

#include "Entities/Entity.h"
#include "Globals.h"

class Event;

class TopDownPlayer : public Entity
{
private:
    public:
    TopDownPlayer(Vector2 startPos, Vector2 size, float moveSpeed, uint16_t color);
    ~TopDownPlayer() = default;
};