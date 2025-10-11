#pragma once

#include "Entities/Entity.h"

struct Event;

class PongManager : public Entity
{
    int scorePlayerOne = 0;
    int scorePlayerTwo = 0;

    void BallOutOfBounds(const Event &event);

    public: 
    PongManager();
    ~PongManager() = default;
};