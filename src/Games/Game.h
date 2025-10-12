#pragma once
#include "Hardware/Input.h"
#include "Hardware/Renderer.h"
#include <string>

class GameManager;

class Game
{
    public:
    std::string myName;
    GameManager& myGameManager;
    
    public:
    Game(std::string name, GameManager& manager) : myName(name), myGameManager(manager) {};

    std::string GetName() const { return myName; };
    virtual void Update(Input& input, float deltaTime) = 0;
    virtual void Render(Renderer& renderer) = 0;
};


