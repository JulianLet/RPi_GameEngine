#include "Systems/AISystem.h"

#include "Entities/Components/AI.h"
#include "Entities/Entity.h"


void AISystem::Update(const std::vector<std::unique_ptr<Entity>> &entities, float deltaTime)
{
    for (auto& entity : entities)
    {
        AIComponent* myAI = entity->GetComponent<AIComponent>();

        if (!myAI) continue;

        if (myAI->think) myAI->think(entity.get(), deltaTime);
        if (myAI->decide) myAI->decide(entity.get(), deltaTime);
        if (myAI->act) myAI->act(entity.get(), deltaTime);
    }
}