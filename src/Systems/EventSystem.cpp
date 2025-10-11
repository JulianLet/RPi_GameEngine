#include "EventSystem.h"

void EventSystem::AddListener(EventListener *listener)
{
    listeners.push_back(listener);
}

void EventSystem::DispatchEvent(const Event &event)
{
    for (EventListener* listener : listeners)
    {
        listener->HandleEvent(event);
    }
}
