#pragma once

#include <cstdint>

class Pong;
class Input;
struct World;

class PongActionSystem
{
    Pong* gameRef;
    uint8_t ballID;
    uint8_t scoreTextID;

    int scorePlayerOne = 0;
    int scorePlayerTwo = 0;

    public: 
    PongActionSystem(Pong* pong);
    ~PongActionSystem() = default;

    void Initialize(World& world, uint8_t ballID, uint8_t scoreTextID);
    void Update(World& world, Input& input);
    void UpdateText(World& world, bool playerOneScored);
    void SetText(World& world);
};