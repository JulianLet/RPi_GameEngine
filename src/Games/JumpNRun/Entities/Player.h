#pragma once

#include "Entities/Entity.h"
#include "Globals.h"

class Event;

class Player : public Entity
{
private:
    Vector2 size = {5, 8};
    float speed = 40.f;
    uint16_t color = Color::BLUE;
    bool isGrounded = false;

    public:
    Player(Vector2 startPos);
    ~Player() = default;

    private:
    void OnCollisionEnter(Entity* self, Entity* other);
    void OnCollisionExit(Entity* self, Entity* other);
    void Jump(const Event& event);
};