#pragma once

#include "Entities/Entity.h"
#include "Hardware/Input.h"

class Pong;
class AIComponent;
class InputIntendComponent;
class PhysicsComponent;
class TransformComponent;
class MovementComponent;

class Paddle : public Entity
{
private:
    bool isPlayer = false;
    Vector2 size = {5, 30};
    float speed = 40.f;
    uint16_t color = Color::BLUE;

    void OnCollisionEnter(Entity* self, Entity* other);

    Pong* pongRef = nullptr;
    AIComponent* myAI = nullptr;
    TransformComponent* myTransform = nullptr;
    InputIntendComponent* myIntend = nullptr;
    TransformComponent* ballTransform = nullptr;
    PhysicsComponent* ballPhysics = nullptr;

    public:
    Paddle(Pong* pongRef, Vector2 startPos, KEYCODE up, KEYCODE down, bool isPlayer);
    ~Paddle() = default;

    private:
    //AI functions
    void Think(Entity* self, float deltaTime);
    void Decide(Entity* self, float deltaTime);
    void Act(Entity* self, float deltaTime);
};