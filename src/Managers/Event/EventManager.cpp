#include "EventManager.h"
#include <algorithm>

#include "EventListener.h"
#include "Games/Game.h"


void EventManager::AddListener(EventListener *listener, Game *game)
{
    listenerEntries.emplace_back(ListenerEntry{listener, game});
}

void EventManager::DispatchEvent(const Event &event)
{
    for (auto& listener : listenerEntries)
    {
        listener.listener->HandleEvent(event);
    }
}

//otherwise saved as nullptr if game gets destroyed
void EventManager::RemoveListenersForGame(Game* game)
{
    listenerEntries.erase(
        std::remove_if(listenerEntries.begin(), listenerEntries.end(),
            [game](const ListenerEntry& e){ return e.ownerGame == game; }),
        listenerEntries.end()
    );
}

