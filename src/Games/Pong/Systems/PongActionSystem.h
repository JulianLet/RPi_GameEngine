#pragma once

#include "Events/EventListener.h"

#include <vector>
#include <memory>

class Event;
class Entity;
class Pong;

class PongActionSystem : public EventListener
{
    Pong* pongRef;

    public: 
    PongActionSystem(Pong* pongRef);
    ~PongActionSystem() = default;

    void Update(const std::vector<std::unique_ptr<Entity>> &entities, Pong &pongRef);
    void HandleEvent(const Event& event) override;
};