#pragma once
#include "Globals.h"
#include "Entities/Entity.h"

class Ball : public Entity
{
private:
    Vector2 startPos = {64,80};
    Vector2 size = {3,3};
    float speed = 40.f;
    uint16_t color = Color::WHITE;

    void OnCollisionEnter(Entity* self, Entity* other);


    public:
    Ball();
    ~Ball() = default;
};