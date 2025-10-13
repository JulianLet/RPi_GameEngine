#include "EventSystem.h"
#include <algorithm>

#include "EventListener.h"
#include "Games/Game.h"


void EventSystem::AddListener(EventListener *listener, Game *game)
{
    listenerEntries.emplace_back(ListenerEntry{listener, game});
}

void EventSystem::DispatchEvent(const Event &event)
{
    for (auto& listener : listenerEntries)
    {
        listener.listener->HandleEvent(event);
    }
}

void EventSystem::RemoveListenersForGame(Game* game)
{
    listenerEntries.erase(
        std::remove_if(listenerEntries.begin(), listenerEntries.end(),
            [game](const ListenerEntry& e){ return e.ownerGame == game; }),
        listenerEntries.end()
    );
}

