#pragma once

#include "Entities/Entity.h"

class JRManager : public Entity
{
    private:
    void CheckHighScore();
    
    public:
    JRManager();
    ~JRManager() = default;


    float bestTime = 0;
    float currentTime = 0;
};