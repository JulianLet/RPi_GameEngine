#pragma once

#include "Entities/Entity.h"

class JRManager : public Entity
{
    private:
    void CheckHighScore(float thisTime);
    
    public:
    JRManager();
    ~JRManager() = default;
    
    float bestTime = 0;
};