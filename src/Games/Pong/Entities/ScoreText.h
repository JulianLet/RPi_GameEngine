#pragma once

#include "Entities/Entity.h"

struct Event;

class ScoreText : public Entity
{
    Vector2 centerPosition;
    char buffer[16]; 

    Vector2 CenteredText(const char* text);
    void UpdateScore(const Event& event);

    public:
    ScoreText(Vector2 position, uint16_t textColor);
    ~ScoreText() = default;

};