#pragma once

#include <vector>

class EventListener;
class Event;
class Game;

struct ListenerEntry {
    EventListener* listener;
    Game* ownerGame; // nullptr = persistent across games
};


class EventSystem
{
private:
    std::vector<ListenerEntry> listenerEntries;
    EventSystem() = default; //is private for singelton

    EventSystem(const EventSystem&) = delete;
    EventSystem& operator=(const EventSystem&) = delete;

public:
    static EventSystem& GetInstance()
    {
        static EventSystem instance;
        return instance;
    }

    void AddListener(EventListener* listener, Game* game);
    void DispatchEvent(const Event& event);
    void RemoveListenersForGame(Game* game);
};