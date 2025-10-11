#pragma once

#include "Events/EventListener.h"

#include <vector>
#include <memory>

class Entity;


class EntityManager : public EventListener
{
    std::vector<std::unique_ptr<Entity>>* entities;
    std::vector<std::unique_ptr<Entity>> entitiesToSpawn;
    
    public:
    EntityManager(std::vector<std::unique_ptr<Entity>>& entities);
    ~EntityManager() = default;

    void Update();
    void HandleEvent(const Event& event) override;
    void SortEntities();
};