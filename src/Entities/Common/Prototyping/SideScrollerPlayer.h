#pragma once

#include "Entities/Entity.h"
#include "Globals.h"

class Event;

class SideScrollerPlayer : public Entity
{
private:
    bool isGrounded = false;
    float jumpPower = 0;

    public:
    SideScrollerPlayer(Vector2 startPos, Vector2 size, float moveSpeed, uint16_t color, float jumpPower);
    ~SideScrollerPlayer() = default;

    private:
    void OnCollisionEnter(Entity* self, Entity* other);
    void OnCollisionExit(Entity* self, Entity* other);
    void Jump(const Event& event);
};