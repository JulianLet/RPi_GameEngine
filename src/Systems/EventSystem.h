#pragma once
#include "Events/EventListener.h"

#include <vector>

class EventSystem
{
private:
    std::vector<EventListener*> listeners;
    EventSystem() = default; //is private for singelton

    EventSystem(const EventSystem&) = delete;
    EventSystem& operator=(const EventSystem&) = delete;

public:
    static EventSystem& GetInstance()
    {
        static EventSystem instance;
        return instance;
    }

    void AddListener(EventListener* listener);
    void DispatchEvent(const Event& event);
};