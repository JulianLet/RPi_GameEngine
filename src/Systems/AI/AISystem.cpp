#include "AISystem.h"

#include "Managers/Game/World.h"
#include "Entities/Component.h"


void AISystem::Update(World& world, float deltaTime)
{
    for (uint8_t e = 0; e < MAX_ENTITIES; e++)
    {
        if (!(world.entities[e].mask & AIBit)) continue;

        auto& myAI = world.ai[e];

        if (myAI.think) myAI.think(world, e, deltaTime);
        if (myAI.decide) myAI.decide(world, e, deltaTime);
        if (myAI.act) myAI.act(world, e, deltaTime);
    }
}