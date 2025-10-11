#pragma once
#include "Entities/Component.h"
#include "Common.h"

#include <string>
#include <unordered_map>
#include <memory>

class Entity
{
    private:
    std::unordered_map<ComponentID, std::unique_ptr<Component>> myComponents;

public:
    std::string tag = "";
    bool isAlive = true;
    bool shouldDelete = false;

    Entity(std::string tag = "Default") : tag(tag) {};

    template<typename T, typename... Args>
    T* AddComponent(Args&&... args)
    {
        T* comp = new T(std::forward<Args>(args)...);
        comp->owner = this;
        myComponents[GetComponentID<T>()] = std::unique_ptr<Component>(comp);
        return comp;
    }

    template<typename T>
    T* GetComponent()
    {
        ComponentID id = GetComponentID<T>();
        auto it = myComponents.find(id);

        if (it != myComponents.end())
        {
            return static_cast<T*>(it->second.get());
        } 

        return nullptr;
    }

    void Reset()
    {
        for (auto& [id, comp] : myComponents)
        {
            comp->Reset();
        }
    }
};