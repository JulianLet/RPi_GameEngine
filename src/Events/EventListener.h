#pragma once

class Event;

class EventListener 
{
public:
    virtual void HandleEvent(const Event& event) = 0;
};
    