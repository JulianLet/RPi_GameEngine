#pragma once

#include <vector>

class EventListener;
class Event;
class Game;

struct ListenerEntry {
    EventListener* listener;
    Game* ownerGame; // nullptr = persistent across games
};


class EventManager
{
private:
    std::vector<ListenerEntry> listenerEntries;
    EventManager() = default; //is private for singelton

    EventManager(const EventManager&) = delete;
    EventManager& operator=(const EventManager&) = delete;

public:
    static EventManager& GetInstance()
    {
        static EventManager instance;
        return instance;
    }

    void AddListener(EventListener* listener, Game* game);
    void DispatchEvent(const Event& event);
    void RemoveListenersForGame(Game* game);
};