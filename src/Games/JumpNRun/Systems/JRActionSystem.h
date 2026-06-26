#pragma once

#include "Managers/Managers.h"

class World;
class JumpNRun;

class JRActionSystem : public EventListener
{
    JumpNRun* gameRef = nullptr;
    float currentBestTime = 99999.f;

    public: 
    JRActionSystem() = default;
    JRActionSystem(JumpNRun* jrRef);
    ~JRActionSystem() = default;

    void Update(World& world, Input& input);
    void HandleEvent(const Event& event) override;
};