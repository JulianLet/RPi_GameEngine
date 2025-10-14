#pragma once

#include "Systems/Events/EventListener.h"

#include <vector>
#include <memory>

class Event;
class Entity;
class Player;
class JumpNRun;
class JRManager;

class JRActionSystem : public EventListener
{
    JumpNRun* jrRef;
    JRManager* manager;

    public: 
    JRActionSystem(JumpNRun* jrRef, JRManager* manager);
    ~JRActionSystem() = default;

    void Update(const std::vector<std::unique_ptr<Entity>> &entities, float deltaTime);
    void HandleEvent(const Event& event) override;
};