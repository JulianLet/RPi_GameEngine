#include "EntityManager.h"

#include "Entities/Entity.h"
#include "Entities/Components/Render/RenderableComponent.h"

#include "Systems/Events/Event.h"
#include "Systems/Events/EventSystem.h"

#include "Games/Game.h"

#include <algorithm>

EntityManager::EntityManager(std::vector<std::unique_ptr<Entity>> &entities, Game* game)
 : entities(&entities)
{
    EventSystem::GetInstance().AddListener(this, game);
}

void EntityManager::Update()
{
    bool somethingChanged = false;

    //delete all entities set to be deleted
    entities->erase(std::remove_if(entities->begin(),entities->end(),
        [](const std::unique_ptr<Entity>& e) 
        {
            return e->shouldDelete;
        }), entities->end());

    //add all new entities
    for (auto& entity : entitiesToSpawn)
    {
        entities->emplace_back(std::move(entity));
        somethingChanged = true;
    }

    entitiesToSpawn.clear(); //is necessary because it will keep nullptr after std::move

    //sort for correct drawing order if something updated
    if (somethingChanged)
    {
        SortEntities();
    }
}

void EntityManager::HandleEvent(const Event& event)
{
    if (event.GetEventType() == EventType::SPAWN_ENTITY)
    {
        const EventSpawnEntity& e = static_cast<const EventSpawnEntity&>(event);
        entitiesToSpawn.emplace_back(std::unique_ptr<Entity>(e.entity)); // take ownership
    }
}


void EntityManager::SortEntities()
{
    std::stable_sort(entities->begin(), entities->end(),
    [](const std::unique_ptr<Entity>& a, const std::unique_ptr<Entity>& b)
    {
        RenderableComponent* ra = a->GetComponent<RenderableComponent>();
        RenderableComponent* rb = b->GetComponent<RenderableComponent>();

        int la = ra ? ra->layer : 0;
        int lb = rb ? rb->layer : 0;
        return la < lb;
    });
}